#include "IrcServer.hpp"
#include "libft.hpp"

IrcServerConfig::IrcServerConfig(){}

IrcServerConfig::IrcServerConfig(Config &cfg):
configfile(cfg.configfile()),
servername(cfg.servername()),
shortinfo(cfg.shortinfo()),
maxChannels(cfg.maxChannels()),
maxMasks(cfg.maxMasks()),
ping(cfg.ping()),
pong(cfg.pong()),
flood(cfg.floodControl()),
pass(cfg.serverpass())
{
	ft::file_to_data(cfg.motdfile(), motd);
}

IrcServerConfig::~IrcServerConfig() {}

CommandStats::CommandStats() :
count(0), byteCount(0)
{}

const std::string IrcServer::_version = "1.0.0";
const size_t IrcServer::maxLineSize = 2048;

IrcServer::IrcServer() :
_state(ALIVE),
creation(::time(NULL)),
_log(std::cerr.rdbuf()),
init(true)
{
	creationDate = ft::to_date(creation, "%a %b %d %Y at %H:%M:%S %Z");
	serviceCommands["KILL"] = &IrcServer::kill;
	serviceCommands["MODE"] = &IrcServer::mode;
	serviceCommands["NICK"] = &IrcServer::nick;
	serviceCommands["NOTICE"] = &IrcServer::notice;
	serviceCommands["OPER"] = &IrcServer::oper;
	serviceCommands["PASS"] = &IrcServer::pass;
	serviceCommands["PING"] = &IrcServer::ping;
	serviceCommands["PONG"] = &IrcServer::pong;
	serviceCommands["PRIVMSG"] = &IrcServer::privmsg;
	serviceCommands["QUIT"] = &IrcServer::quit;
	serviceCommands["SERVICE"] = &IrcServer::service;
	serviceCommands["SERVLIST"] = &IrcServer::servlist;
	serviceCommands["SQUERY"] = &IrcServer::squery;
	serviceCommands["USER"] = &IrcServer::user;
	serviceCommands["WHO"] = &IrcServer::who;
	serviceCommands["WHOIS"] = &IrcServer::whois;
	serviceCommands["WHOWAS"] = &IrcServer::whowas;
	userCommands = serviceCommands;
	userCommands["AWAY"] = &IrcServer::away;
	userCommands["DIE"] = &IrcServer::die;
	userCommands["INFO"] = &IrcServer::info;
	userCommands["INVITE"] = &IrcServer::invite;
	userCommands["JOIN"] = &IrcServer::join;
	userCommands["KICK"] = &IrcServer::kick;
	userCommands["LIST"] = &IrcServer::list;
	userCommands["LUSERS"] = &IrcServer::lusers;
	userCommands["MOTD"] = &IrcServer::motd;
	userCommands["NAMES"] = &IrcServer::names;
	userCommands["PART"] = &IrcServer::part;
	userCommands["REHASH"] = &IrcServer::rehash;
	userCommands["RESTART"] = &IrcServer::restart;
	userCommands["STATS"] = &IrcServer::stats;
	userCommands["TIME"] = &IrcServer::time;
	userCommands["TOPIC"] = &IrcServer::topic;
	userCommands["USERHOST"] = &IrcServer::userhost;
	userCommands["VERSION"] = &IrcServer::version;
}

IrcServer::~IrcServer()
{
	if (logfile.is_open())
		logfile.close();
}

IrcServer::State IrcServer::state() const { return _state; }

std::ostream &IrcServer::log() throw() { return _log << ft::to_date(::time(NULL), "%x - %I:%M:%S "); }

void IrcServer::run()
{
	while (_state == ALIVE)
	{
		try { srv.select(); } catch(tcp::TcpServer::SigintException &e) { _state = DIE; return ; }
		tcp::TcpSocket *newSocket;
		while ((newSocket = srv.nextNewConnection()))
		{
			User *u = new User(newSocket, config.pass.size() ? UserRequirement::ALL : UserRequirement::ALL_EXCEPT_PASS);
			writeMessage(*u, "NOTICE", "Connection established");
			log() << u->socket()->host() << " CONNECTED" << std::endl;
			network.add(u);
		}
		police();
		tcp::TcpSocket *socket;
		while ((socket = srv.nextPendingConnection()) && _state == ALIVE)
		{
			try
			{
				socket->flush();
				while (socket->canReadLine())
				{
					std::string line;
					if (!socket->readLine(line))
					{
						disconnect(socket, "Remote host closed connection");
						continue ;
					}
					if (socket->readBufSize() > maxLineSize)
					{
						disconnect(socket, "Socket's buffer size has exceed the limit");
						continue ;
					}
					if (line.empty())
						continue ;
					exec(network.getBySocket(socket), IRC::Message(line));
				}
			}
			catch (std::exception &e) {
				log() << e.what() << std::endl;
				disconnect(socket, e.what());
				continue ;
			}
		}
	}
}

void IrcServer::loadConfig(const std::string &file)
{
	Config cfg(file);

	if (init)
	{
		logfile.open(cfg.logfile().c_str(), std::ios::out);
		if (!logfile.fail())
			_log.rdbuf(logfile.rdbuf());
		srv.listen(cfg.tcpPort().c_str());
		if (cfg.sslPort().size())
		{
			srv.loadSslConfig(cfg.certFile(), cfg.keyFile());
			srv.listen(cfg.sslPort().c_str(), true);
		}
		init = false;
	}
	srv.setMaxConnections(cfg.maxConnections());
	cfg.opers(network.opers());
	cfg.fnicks(network.fnicks());
	network.setHistorySize(cfg.historySize());
	config = IrcServerConfig(cfg);
}

void IrcServer::disconnect(tcp::TcpSocket *socket, const std::string &reason) throw()
{
	BasicConnection *c = network.getBySocket(socket);
	disconnect(*static_cast<User *>(c), reason);
}


void IrcServer::disconnect(User &u, const std::string &reason, bool notifyUserQuit) throw()
{
	const std::string quitMessage = (IRC::MessageBuilder(u.prefix(), "QUIT") << reason).str();
	std::stringstream errorReason;

	if (u.joinedChannels())
	{
		network.resetUserReceipt();
		const Network::ChannelMap &channels = network.channels();
		Network::ChannelMap::const_iterator i = channels.begin();
		while (i != channels.end())
		{
			Channel *c = i->second;
			++i;
			if (c->findMember(&u))
			{
				c->delMember(&u);
				if (!c->count())
					network.remove(c);
				else
				{
					c->send(quitMessage, NULL, true);
					c->markAllMembers();
				}
			}
		}
	}
	errorReason << "Closing Link: " << u.socket()->host();
	if (notifyUserQuit)
		errorReason << " (Client Quit)";
	else
		errorReason << " (" << reason << ')';
	writeError(u.socket(), errorReason.str());
	network.remove(&u);
	network.newZombie(&u);
	log() << u.socket()->host() << ' ' << errorReason.str() << std::endl;
}

int IrcServer::exec(BasicConnection *sender, const IRC::Message &msg)
{
	log() << msg << std::endl;
	if (!msg.isValid())
		return (-1);
	User &u = *static_cast<User*>(sender);
	if (!floodControl(u))
		return (0);
	userCommandsMap &map = u.type() == User::USER ? userCommands : serviceCommands;
	userCommandsMap::const_iterator i = map.find(msg.command());
	if (i == map.end())
		return (writeNum(u, IRC::Error::unknowncommand(msg.command())));
	int commandStatus = (this->*(i->second))(u, msg);
	CommandStats &stats = commandsStats[msg.command()];
	++stats.count;
	stats.byteCount += msg.entry().size();
	return (commandStatus);
}

void IrcServer::writeMessage(User &dst, const std::string &command, const std::string &content)
{
	dst.writeLine((IRC::MessageBuilder(config.servername, command) << dst.nickname() << content).str());
}

int IrcServer::writeNum(User &dst, const  IRC::Numeric &response)
{
	dst.writeLine(IRC::MessageBuilder(config.servername, response, dst.nickname()).str());
	return (-1);
}

void IrcServer::writeMotd(User &u)
{
	if (!config.motd.size())
	{
		writeNum(u, IRC::Error::nomotd());
		return ;
	}
	writeNum(u, IRC::Reply::motdstart(config.servername));
	writeNum(u, IRC::Reply::motd(config.motd.front()));
	for (std::vector<std::string>::iterator it = ++config.motd.begin(); it != config.motd.end(); ++it)
		writeNum(u, IRC::Reply::motd(*it));
	writeNum(u, IRC::Reply::endofmotd());
}

void IrcServer::writeWelcome(User &u)
{
	writeNum(u, (u.type() == User::USER ? IRC::Reply::welcome(u.prefix()) : IRC::Reply::youreservice(u.nickname())));
	writeNum(u, IRC::Reply::yourhost(config.servername, _version));
	writeNum(u, IRC::Reply::created(creationDate));
	writeNum(u, IRC::Reply::myinfo(config.servername, _version, "Oaiorsw", "IObeiklmnopstv"));
	if (u.type() == User::USER)
		writeMotd(u);
}

void IrcServer::writeError(tcp::TcpSocket *s, std::string reason)
{
	s->writeLine((IRC::MessageBuilder(config.servername, "ERROR") << reason).str());
}

void IrcServer::police()
{
	static time_t ptime = 0;
	time_t ctime = ::time(NULL);
	BasicConnection *z;

	while ((z = network.nextZombie()))
	{
		try { z->socket()->flush(); } catch(std::exception &e) {}
		srv.disconnect(z->socket());
		delete z;
	}
	if (ctime - ptime < 5)
		return ;
	ptime = ctime;
	for (Network::ConnectionMap::const_iterator it = network.connections().begin(); it != network.connections().end();) {
		BasicConnection *c = it->second;
		it++;
		if (c->pongExpected() && ctime - c->clock() > config.pong)
			disconnect(c->socket(), "Ping timeout");
		else if (!c->pongExpected() && ctime - c->clock() > config.ping)
		{
			c->writeLine("PING :" + config.servername);
			c->clock() = ctime;
			c->pongExpected() = true;
		}
	}
}

bool IrcServer::floodControl(User &u)
{
	if (u.pongExpected() || config.flood == false)
		return true;
	time_t ctime = ::time(NULL);
	if (u.clock() - ctime >= 10)
		return false;
	if (u.clock() < ctime)
		u.clock() = ctime;
	u.clock() += 2;
	return true;
}
