/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/28 21:13:51 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

IrcServerConfig::IrcServerConfig(size_t maxChannels) :
maxChannels(maxChannels)
{}

IrcServer::IrcServer() :
config(IrcServerConfig(5))
{
	load("AWAY", BasicConnection::USER, &IrcServer::away);
	load("JOIN", BasicConnection::USER, &IrcServer::join);
	load("NICK", BasicConnection::USER, &IrcServer::userNick);
	load("NICK", BasicConnection::UNKNOWN, &IrcServer::unknownNick);
	load("PRIVMSG", BasicConnection::USER, &IrcServer::privmsg);
	load("TOPIC", BasicConnection::USER, &IrcServer::topic);
	load("USER", BasicConnection::USER, &IrcServer::userUser);
	load("USER", BasicConnection::UNKNOWN, &IrcServer::unknownUser);
}

IrcServer::~IrcServer() {}

void IrcServer::setMaxConnections(size_t MaxConnections) { srv.setMaxConnections(MaxConnections); }

void IrcServer::setVerbose(bool verbose) { srv.setVerbose(verbose); }

void IrcServer::setLogDestination(const std::string &destfile) { srv.setLogDestination(destfile); }

std::ostream &IrcServer::log() throw() { return srv.log(); }

void IrcServer::listen(const char *port, SSL_CTX *ctx, size_t maxQueueLen) { srv.listen(port, ctx, maxQueueLen); }

void IrcServer::disconnect(BasicConnection *connection) throw()
{
	if (connection->type() == BasicConnection::USER)
	{
		User *usr = network.conToUsr(connection);
		network.removeUser(usr);
	}
	else if (connection->type() == BasicConnection::SERVER)
	{
		RemoteServer *srv = network.conToSrv(connection);
		network.removeServer(srv);
	}
	network.removeConnection(connection);
	srv.disconnect(connection->socket());
	delete connection;
}

void IrcServer::disconnect(TcpSocket *socket) throw()
{
	BasicConnection *connection;

	if (!(connection = network.getConnection(socket)))
	{
		srv.disconnect(socket);
		log() << "Unknown connection detected, disconnecting" << std::endl;
	}
	disconnect(connection);
}

void IrcServer::load(const std::string &cmd, BasicConnection::Type type, cmdType handler)
{
	commands[cmdIdType(cmd, type)] = handler;
}

BasicConnection *IrcServer::findSender(const Prefix &prefix, TcpSocket *socket)
{
	BasicConnection *connection = network.getConnection(socket);
	if (!connection || connection->type() != BasicConnection::SERVER)
		return connection;
	if (!prefix.nickname().empty())
			return (network.getUser(prefix.nickname()));
	if (!prefix.servername().empty())
			return (network.getServer(prefix.servername()));
	return NULL;
}

void IrcServer::exec(BasicConnection *sender, const Message &msg)
{
	cmdMapType::iterator it;

	if ((it = commands.find(cmdIdType(msg.command(), sender->type()))) == commands.end())
	{
		if (sender->type() != BasicConnection::UNKNOWN)
			sender->writeLine("Unknown Command");
		return ;
	}
	(this->*(it->second))(sender, msg);
}

void IrcServer::run() throw()
{
	while (1)
	{
		try { srv.select(); } catch(TcpServer::SigintException &e) { log() << "\rSIGINT catched, exiting properly" << std::endl; return ; }
		TcpSocket *newSocket;
		while ((newSocket = srv.nextNewConnection()))
		{
			newSocket->writeLine("Connection established");
			network.newConnection(newSocket);
		}
		TcpSocket *socket;
		while ((socket = srv.nextPendingConnection()))
		{
			try { if (!socket->IO()) { disconnect(socket); continue ; } }
			catch (std::exception &e) { log() << e.what() << std::endl; disconnect(socket); continue ; }
			std::string line;
			if (!socket->readLine(line))
				continue ;
			Message msg(line);
			if (!msg.isValid())
				continue ;
			log() << msg << std::endl;
			BasicConnection *sender;
			if (!(sender = findSender(msg.prefix(), socket)))
				{ log() << "Unknwon Sender, disconnecting" << std::endl; disconnect(socket); continue ; } //disconnect if msg of unknown origin
			exec(sender, msg);
		}
	}
}
