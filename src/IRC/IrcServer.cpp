#include "IrcServer.hpp"

IrcServerConfig::IrcServerConfig(){}

IrcServerConfig::IrcServerConfig(Config &cfg):
	maxChannels(cfg.maxChannels()),
	maxMasks(cfg.maxMasks()),
	ping(cfg.ping()),
	pong(cfg.pong()),
	flood(cfg.floodControl())
{
	cfg.servername(servername);
	cfg.motdfile(motdfile);
}

CommandStats::CommandStats() :
count(0), byteCount(0)
{}

IrcServer::IrcServer() :
creation(::time(NULL))
{
	creationDate = Utils::to_date(creation, "%a %b %d %Y at %H:%M:%S %Z");
	userCommands["AWAY"] = &IrcServer::away;
	userCommands["INVITE"] = &IrcServer::invite;
	userCommands["JOIN"] = &IrcServer::join;
	userCommands["KICK"] = &IrcServer::kick;
	userCommands["KILL"] = &IrcServer::kill;
	userCommands["LIST"] = &IrcServer::list;
	userCommands["LUSERS"] = &IrcServer::lusers;
	userCommands["MODE"] = &IrcServer::mode;
	userCommands["MOTD"] = &IrcServer::motd;
	userCommands["NAMES"] = &IrcServer::names;
	userCommands["NICK"] = &IrcServer::nick;
	userCommands["NOTICE"] = &IrcServer::notice;
	userCommands["OPER"] = &IrcServer::oper;
	userCommands["PART"] = &IrcServer::part;
	userCommands["PASS"] = &IrcServer::pass;
	userCommands["PING"] = &IrcServer::ping;
	userCommands["PONG"] = &IrcServer::pong;
	userCommands["PRIVMSG"] = &IrcServer::privmsg;
	userCommands["QUIT"] = &IrcServer::quit;
	userCommands["STATS"] = &IrcServer::stats;
	userCommands["TIME"] = &IrcServer::time;
	userCommands["TOPIC"] = &IrcServer::topic;
	userCommands["USER"] = &IrcServer::user;
	userCommands["VERSION"] = &IrcServer::version;
	userCommands["WHO"] = &IrcServer::who;
	userCommands["WHOIS"] = &IrcServer::whois;
	userCommands["WHOWAS"] = &IrcServer::whowas;
}

IrcServer::~IrcServer() {}

void IrcServer::listen(const char *port, SSL_CTX *ctx, size_t maxQueueLen) { srv.listen(port, ctx, maxQueueLen); }

std::ostream &IrcServer::log() throw() { return srv.log(); }

void IrcServer::run() throw()
{
	while (1)
	{
		try { srv.select(); } catch(TcpServer::SigintException &e) { log() << "\rSIGINT catched, exiting properly" << std::endl; return ; }
		TcpSocket *newSocket;
		while ((newSocket = srv.nextNewConnection()))
		{
			User *u = new User(newSocket, UserRequirement::ALL_EXCEPT_PASS);
			writeMessage(*u, "NOTICE", "Connection established");
			network.add(u);
		}
		police();
		TcpSocket *socket;
		while ((socket = srv.nextPendingConnection()))
		{
			try {
				if (!socket->IO())
				{
					disconnect(socket, "Remote host closed connection");
					continue ;
				}
			}
			catch (std::exception &e) {
				log() << e.what() << std::endl;
				disconnect(socket, "Corrupt link");
				continue ;
			}
			std::string line;
			if (!socket->readLine(line))
				continue ;
			exec(network.getBySocket(socket), IRC::Message(line));
		}
	}
}

void IrcServer::setConfig(Config &cfg)
{
	srv.setMaxConnections(cfg.maxConnections());
	srv.setVerbose(cfg.verbose());
	srv.setLogDestination(cfg.logfile());
	cfg.opers(network.opers());
	cfg.fnicks(network.fnicks());
	config = IrcServerConfig(cfg);
}

void IrcServer::disconnect(TcpSocket *socket, const std::string &reason) throw()
{
	BasicConnection *c = network.getBySocket(socket);
	if (c->type() == BasicConnection::USER)
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
	if (!msg.isValid())
		return (-1);
	if (sender->type() == BasicConnection::USER)
	{
		User &u(*static_cast<User*>(sender));
		if (!floodControl(u))
			return (0);
		userCommandsMap::const_iterator i(userCommands.find(msg.command()));
		if (i == userCommands.end())
			return (writeNum(u, IRC::Error::unknowncommand(msg.command())));
		int commandStatus = (this->*(i->second))(u, msg);
		CommandStats &stats = commandsStats[msg.command()];
		++stats.count;
		stats.byteCount += msg.entry().size();
		return (commandStatus);
	}
	return (-1);
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
	writeNum(u, IRC::Reply::welcome(u.prefix()));
	writeNum(u, IRC::Reply::yourhost(config.servername, config.version));
	writeNum(u, IRC::Reply::created(creationDate));
	writeNum(u, IRC::Reply::myinfo(config.servername, config.version, "Oaiorsw", "IObeiklmnopstv"));
	writeMotd(u);
}

void IrcServer::writeError(TcpSocket *s, std::string reason)
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
		try { z->socket()->IO(); } catch(std::exception &e) {}
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
	std::ifstream f(config.motdfile.c_str(), std::ios_base::in);
	std::string line;
	config.motd.clear();
	while (std::getline(f, line))
	{
		if (line.size() > 80)
			line.resize(80);
		config.motd.push_back(line);
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
