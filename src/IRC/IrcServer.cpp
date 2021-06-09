/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/06/09 05:27:30 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

IrcServerConfig::IrcServerConfig(){}

IrcServerConfig::IrcServerConfig(Config &cfg):
	version(cfg.version()),
	servername(cfg.servername()),
	maxChannels(cfg.maxChannels()),
	ping(cfg.ping()),
	pong(cfg.pong()),
	flood(cfg.floodControl()),
	motdfile(cfg.motdfile()) {}

IrcServer::IrcServer()
{
	time_t	current;
	char	buf[80];

	userCommands["AWAY"] = &IrcServer::away;
	userCommands["INVITE"] = &IrcServer::invite;
	userCommands["JOIN"] = &IrcServer::join;
	userCommands["LIST"] = &IrcServer::list;
	userCommands["MODE"] = &IrcServer::mode;
	userCommands["MOTD"] = &IrcServer::motd;
	userCommands["NAMES"] = &IrcServer::names;
	userCommands["NICK"] = &IrcServer::nick;
	userCommands["NOTICE"] = &IrcServer::notice;
	userCommands["OPER"] = &IrcServer::oper;
	userCommands["PART"] = &IrcServer::part;
	userCommands["PING"] = &IrcServer::ping;
	userCommands["PONG"] = &IrcServer::pong;
	userCommands["PRIVMSG"] = &IrcServer::privmsg;
	userCommands["QUIT"] = &IrcServer::quit;
	userCommands["TIME"] = &IrcServer::time;
	userCommands["TOPIC"] = &IrcServer::topic;
	userCommands["USER"] = &IrcServer::user;

	::time(&current);
	strftime(buf, sizeof(buf), "%a %b %d %Y at %H:%M:%S %Z", localtime(&current));
	creationDate = buf;
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
		Police();
		TcpSocket *socket;
		while ((socket = srv.nextPendingConnection()))
		{
			try {
				if (!socket->IO())
				{
					disconnect(socket, "Remote host closed the connection");
					continue ;
				}
			}
			catch (std::exception &e) {
				log() << e.what() << std::endl;
				disconnect(socket, "IO exception");
				continue ;
			}
			std::string line;
			if (!socket->readLine(line))
				continue ;
			exec(network.getBySocket(socket), Message(line));
		}
	}
}

void IrcServer::setConfig(Config &cfg)
{
	srv.setMaxConnections(cfg.maxConnections());
	srv.setVerbose(cfg.verbose());
	srv.setLogDestination(cfg.logfile());
	network.setOpers(cfg.opers());
	config = IrcServerConfig(cfg);
}

void IrcServer::disconnect(TcpSocket *socket, const std::string &reason) throw()
{
	BasicConnection *c = network.getBySocket(socket);
	if (c->type() == BasicConnection::USER)
		disconnect(*static_cast<User *>(c), reason, false);
	else if (c->type() == BasicConnection::SERVER)
		disconnect(*static_cast<RemoteServer *>(c), reason);
}

void IrcServer::disconnect(User &u, const std::string &reason, bool notifyUserQuit) throw()
{
	const std::string quitMessage = (MessageBuilder(u.prefix(), "QUIT") << reason).str();
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
				if (c->count())
				{
					c->send(quitMessage, NULL, true);
					c->markAllMembers();
				}
				else
					network.remove(c);
			}
		}
	}
	if (notifyUserQuit)
		u.writeLine(quitMessage);
	errorReason << "Closing Link: " << u.socket()->host() << " (Client Quit)";
	writeError(u.socket(), errorReason.str());
	network.remove(&u);
	network.newZombie(&u);
}

void IrcServer::disconnect(Server &s, const std::string &reason) throw()
{
	(void)reason;
	network.remove(&s);
	// Must remove all Users using same socket too
	srv.disconnect(s.socket());
	delete (&s);
}

int IrcServer::exec(BasicConnection *sender, const Message &msg)
{
	if (!msg.isValid())
		return (-1);
	log() << "Message: " << msg << std::endl;
	if (sender->type() == BasicConnection::USER)
	{
		User &u(*static_cast<User*>(sender));
		if (!floodControl(u))
			return (0);
		userCommandsMap::const_iterator i(userCommands.find(msg.command()));
		if (i == userCommands.end())
			return (writeNum(u, IrcError::unknowncommand(msg.command())));
		return ((this->*(i->second))(u, msg));
	}
	if (sender->type() == BasicConnection::SERVER)
	{
		serverCommandsMap::iterator i(serverCommands.find(msg.command()));
		if (i != serverCommands.end())
			return ((this->*(i->second))(*static_cast<Server*>(sender), msg));
	}
	return (-1);
}

// std::string goes brrr
static bool match(const char *mask, const char *str)
{
	const char m = *mask;
	const char c = *str;
	if (!m)
		return (!c);
	if (m == '\\') // escape character (only if next is a special character)
	{
		const char m1 = mask[1];
		if (m1 == '*' || m1 == '?')
			return (m1 == c ? match(mask + 2, str + 1) : 0);
	}
	if (m == '*') // wildcard matching
	{
		if (match(mask + 1, str))
			return (1);
		if (!c)
			return (0);
	}
	else
	{
		if (m == '?') // any character matching
		{
			if (!c)
				return (0);
		}
		else if (m != c) // else character must match
			return (0);
		++mask;
	}
	return (match(mask, str + 1));
}

bool IrcServer::match(const std::string &mask, const std::string &str)
{
	return (::match(mask.c_str(), str.c_str()));
}

void IrcServer::writeMessage(User &dst, const std::string &command, const std::string &content)
{
	dst.writeLine((MessageBuilder(config.servername, command) << dst.nickname() << content).str());
}

int IrcServer::writeNum(User &dst, const IrcNumeric &response)
{
	dst.writeLine(MessageBuilder(config.servername, response, dst.nickname()).str());
	return (-1);
}

void IrcServer::writeMotd(User &u)
{
	if (!config.motd.size())
	{
		writeNum(u, IrcError::nomotd());
		return ;
	}
	writeNum(u, IrcReply::motdstart(config.servername));
	writeNum(u, IrcReply::motd(config.motd.front()));
	for (std::list<std::string>::iterator it = ++config.motd.begin(); it != config.motd.end(); ++it)
		writeNum(u, IrcReply::motd(*it));
	writeNum(u, IrcReply::endofmotd());
}

void IrcServer::writeWelcome(User &u)
{
	writeNum(u, IrcReply::welcome(u.prefix()));
	writeNum(u, IrcReply::yourhost(config.servername, config.version));
	writeNum(u, IrcReply::created(creationDate));
	writeNum(u, IrcReply::myinfo(config.servername, config.version, "Oaiorsw", "IOabeiklmnopqrstv"));
	writeMotd(u);
}

void IrcServer::writeError(TcpSocket *s, std::string reason)
{
	s->writeLine((MessageBuilder(config.servername, "ERROR") << reason).str());
}

void IrcServer::Police()
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
		{
			log() << "A connection has not pong in time" << std::endl;
			disconnect(c->socket(), "Ping timeout");
		}
		else if (!c->pongExpected() && ctime - c->clock() > config.ping)
		{
			c->writeLine("PING :" + config.servername);
			c->clock() = ctime;
			c->pongExpected() = true;
			log() << "Ping has been sent" << std::endl;
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
