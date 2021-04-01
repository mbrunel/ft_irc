/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/01 18:54:36 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

IrcServerConfig::IrcServerConfig(const std::string &version, size_t maxChannels) :
version(version),
maxChannels(maxChannels)
{}

IrcServer::IrcServer() :
config(IrcServerConfig("ircserv-1.0.0", 5)),
prefix("irc.server.local")
{
	userCommands["AWAY"] = &IrcServer::away;
	userCommands["JOIN"] = &IrcServer::join;
	userCommands["MOTD"] = &IrcServer::motd;
	userCommands["NICK"] = &IrcServer::nick;
	userCommands["PRIVMSG"] = &IrcServer::privmsg;
	userCommands["TOPIC"] = &IrcServer::topic;
	userCommands["USER"] = &IrcServer::user;
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
			newSocket->writeLine("Connection established");
			// Add check if connection is an authorized server
			network.add(new User(newSocket, UserRequirement::ALL_EXCEPT_PASS));
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
	}
}

void IrcServer::setLogDestination(const std::string &destfile) { srv.setLogDestination(destfile); }

void IrcServer::setMaxConnections(size_t MaxConnections) { srv.setMaxConnections(MaxConnections); }

void IrcServer::setVerbose(bool verbose) { srv.setVerbose(verbose); }

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

void IrcServer::disconnect(RemoteServer *connection) throw()
{
	network.remove(connection);
	// Must remove all Users using same socket too
	srv.disconnect(connection->socket());
	delete (connection);
}

void IrcServer::exec(BasicConnection *sender, const Message &msg)
{
	if (!msg.isValid())
		return ;
	if (sender->type() == BasicConnection::USER)
	{
		userCommandsMap::iterator i;
		if ((i = userCommands.find(msg.command())) != userCommands.end())
		{
			(this->*(i->second))(*static_cast<User*>(sender), msg);
			return ;
		}
	}
	else if (sender->type() == BasicConnection::SERVER)
	{
		serverCommandsMap::iterator i;
		if ((i = serverCommands.find(msg.command())) != serverCommands.end())
		{
			(this->*(i->second))(*static_cast<RemoteServer*>(sender), msg);
			return ;
		}
	}
	else
		return ;
	sender->writeLine("Unknown Command");
}

void IrcServer::writeNum(User &dst, const IrcNumeric &response)
{
	dst.writeNum(prefix, response);
}

void IrcServer::writeMotd(User &u)
{
	writeNum(u, IrcReply::motdstart(prefix));
	writeNum(u, IrcReply::motd("Welcome to our broken IRC server!"));
	writeNum(u, IrcReply::motd("Don't forget to install Discord on your computer next time :)"));
	writeNum(u, IrcReply::endofmotd());
}

void IrcServer::writeWelcome(User &u)
{
	writeNum(u, IrcReply::welcome(u.prefix()));
	writeNum(u, IrcReply::yourhost(prefix, config.version));
	writeNum(u, IrcReply::created("in the past (for sure)"));
	writeNum(u, IrcReply::myinfo(prefix, config.version, "<available user modes>", "<available channel modes>"));
	writeMotd(u);
}

