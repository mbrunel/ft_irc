/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/24 01:04:43 by mbrunel          ###   ########.fr       */
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
flood(cfg.floodControl())
{
	std::ifstream f(cfg.motdfile().c_str(), std::ios_base::in);
	std::string line;
	while (std::getline(f, line))
		motd.push_back(line);
}

IrcServer::IrcServer()
{
	userCommands["AWAY"] = &IrcServer::away;
	userCommands["JOIN"] = &IrcServer::join;
	userCommands["MODE"] = &IrcServer::mode;
	userCommands["MOTD"] = &IrcServer::motd;
	userCommands["NICK"] = &IrcServer::nick;
	userCommands["PART"] = &IrcServer::part;
	userCommands["PRIVMSG"] = &IrcServer::privmsg;
	userCommands["TOPIC"] = &IrcServer::topic;
	userCommands["USER"] = &IrcServer::user;
	userCommands["OPER"] = &IrcServer::oper;
	userCommands["PING"] = &IrcServer::ping;
	userCommands["PONG"] = &IrcServer::pong;
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
			// Add check if connection is an authorized server
			User *u = new User(newSocket, UserRequirement::ALL_EXCEPT_PASS);
			writeMessage(*u, "NOTICE", "Connection established");
			network.add(u);
		}
		TcpSocket *socket;
		while ((socket = srv.nextPendingConnection()))
		{
			try { if (!socket->IO()){ disconnect(socket); continue ; } }
			catch (std::exception &e) { log() << e.what() << std::endl; disconnect(socket); continue ; }
			std::string line;
			if (!socket->readLine(line))
				continue ;
			exec(network.getBySocket(socket), Message(line));
		}
		Police();
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

void IrcServer::disconnect(TcpSocket *socket) throw()
{
	BasicConnection *c = network.getBySocket(socket);
	if (c->type() == BasicConnection::USER)
		disconnect(static_cast<User *>(c));
	else if (c->type() == BasicConnection::SERVER)
		disconnect(static_cast<RemoteServer *>(c));
}

void IrcServer::disconnect(User *connection) throw()
{
	network.remove(connection);
	srv.disconnect(connection->socket());
	delete (connection);
}

void IrcServer::disconnect(Server *connection) throw()
{
	network.remove(connection);
	// Must remove all Users using same socket too
	srv.disconnect(connection->socket());
	delete (connection);
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
	writeNum(u, IrcReply::motdstart(config.servername));
	if (config.motd.size())
	{
		writeNum(u, IrcReply::motd(config.motd.front()));
		for (std::list<std::string>::iterator it = ++config.motd.begin(); it != config.motd.end(); ++it)
			writeNum(u, IrcReply::motd(*it));
	}
	writeNum(u, IrcReply::endofmotd());
}

void IrcServer::writeWelcome(User &u)
{
	writeNum(u, IrcReply::welcome(u.prefix()));
	writeNum(u, IrcReply::yourhost(config.servername, config.version));
	writeNum(u, IrcReply::created("in the past (for sure)"));
	writeNum(u, IrcReply::myinfo(config.servername, config.version, "<available user modes>", "<available channel modes>"));
	writeMotd(u);
}

void IrcServer::Police()
{
	static time_t ptime = 0;
	time_t ctime = time(NULL);
	if (ctime - ptime < 5)
		return ;
	ptime = ctime;
	for (Network::ConnectionMap::const_iterator it = network.connections().begin(); it != network.connections().end();) {
		BasicConnection *c = it->second;
		it++;
		if (c->pongExpected() && ctime - c->clock() > config.pong)
		{
		//	c->writeLine(":" + config.servername + " ERROR :Closing Link: (Ping Timeout)"); can't send msg before disconnection
			log() << "A connection has not pong in time" << std::endl;
			disconnect(c->socket());
		}
		else if (!c->pongExpected() && ctime - c->clock() > config.ping)
		{
			c->writeLine("PING :" + config.servername);
			c->clock() = ctime;
			c->pongExpected() = true;
			log() << "Ping has been sent" << std::endl;
		}
	}
}

bool IrcServer::floodControl(User &u)
{
	if (u.pongExpected() || config.flood == false)
		return true;
	time_t ctime = time(NULL);
	if (u.clock() - ctime >= 10)
		return false;
	if (u.clock() < ctime)
		u.clock() = ctime;
	u.clock() += 2;
	return true;
}
