/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/20 15:46:53 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

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
	userCommands["MODE"] = &IrcServer::mode;
	userCommands["MOTD"] = &IrcServer::motd;
	userCommands["NICK"] = &IrcServer::nick;
	userCommands["PART"] = &IrcServer::part;
	userCommands["PRIVMSG"] = &IrcServer::privmsg;
	userCommands["TOPIC"] = &IrcServer::topic;
	userCommands["USER"] = &IrcServer::user;
	userCommands["OPER"] = &IrcServer::oper;
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
	}
}

void IrcServer::setLogDestination(const std::string &destfile) { srv.setLogDestination(destfile); }

void IrcServer::setMaxConnections(size_t MaxConnections) { srv.setMaxConnections(MaxConnections); }

void IrcServer::setVerbose(bool verbose) { srv.setVerbose(verbose); }

void IrcServer::setOpers(std::map<std::string, Oper> opers) { network.setOpers(opers); }

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
	dst.writeLine((MessageBuilder(prefix, command) << dst.nickname() << content).str());
}

int IrcServer::writeNum(User &dst, const IrcNumeric &response)
{
	dst.writeLine(MessageBuilder(prefix, response, dst.nickname()).str());
	return (-1);
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

