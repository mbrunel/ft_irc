/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/24 00:26:44 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

IrcServer::IrcServer() {}

IrcServer::~IrcServer()
{
	for (nickIter it = allUsers. begin() ; it != allUsers.end() ; ++it)
		if (it->second->isRemote())
			delete it->second;
	for (sockIter it = localUsers.begin() ; it != localUsers.end() ; ++it)
		disconnect(it->second);
}

void IrcServer::setMaxConnections(size_t MaxConnections) { srv.setMaxConnections(MaxConnections); }

void IrcServer::setVerbose(bool verbose) { srv.setVerbose(verbose); }

void IrcServer::setLogDestination(const std::string &destfile) { srv.setLogDestination(destfile); }

std::ostream &IrcServer::log() throw() { return srv.log(); }

void IrcServer::listen(const char *port, SSL_CTX *ctx, size_t maxQueueLen) { srv.listen(port, ctx, maxQueueLen); }

User *IrcServer::userFromConnection(BasicConnection *connection)
{
	User *user;
	if (!(user = dynamic_cast<User *>(connection)))
	{
		log() << "Connection is not a user" << std::endl;
		return NULL;
	}
	return user;
}

void IrcServer::disconnect(BasicConnection *connection) throw()
{
	srv.disconnect(connection->socket());
	if (connection->type() == BasicConnection::USER)
	{
		User *user;
		if ((user = userFromConnection(connection)) && (user->state() == User::NEED_USER || user->state() == User::CONNECTED))
			allUsers.erase(user->nickname());
	}
	localUsers.erase(connection->socket());
	delete connection;
}

void IrcServer::disconnect(TcpSocket *socket) throw()
{
	sockIter it;
	if ((it = localUsers.find(socket)) != localUsers.end())
		disconnect(it->second);
	else
	{
		log() << "We've been hacked somehow" << std::endl;
		srv.disconnect(socket);
	}
}

BasicConnection *IrcServer::findSender(const Prefix &prefix, TcpSocket *Connection)
{
	if (!prefix.nickname().empty())
	{
		nickIter it;
		if ((it = allUsers.find(prefix.nickname())) != allUsers.end())
			return it->second;
		log() << "ERROR Unknown nick" << std::endl;
		return NULL;
	}
	else if (prefix.nickname().empty() && prefix.servername().empty())
	{
		sockIter it;
		if ((it = localUsers.find(Connection)) != localUsers.end())
			return it->second;
		log() << "ERROR Unknown socket" << std::endl;
		return NULL;
	}
	log() << "ERROR Unknown prefix" << std::endl;
	return NULL;
}

void IrcServer::exec(BasicConnection *sender, const Message &msg)
{
	cmdMapType::iterator it;

	if ((it = commands.find(cmdIdType(msg.command(), sender->type()))) == commands.end())
	{
		sender->writeTo("Unknown Command\n");
		return ;
	}
	(this->*(it->second))(sender, msg);
}

void IrcServer::run() throw()
{
	while (1)
	{
		try { srv.select(); } catch(TcpServer::SigintException &e) { log() << "\rSIGINT catched, exiting properly" << std::endl; return ; }
		TcpSocket *newConnection;
		while ((newConnection = srv.nextNewConnection()))
		{
			newConnection->writeLine("Connection established\n");
			localUsers[newConnection] = new User(newConnection);
		}
		TcpSocket *Connection;
		while ((Connection = srv.nextPendingConnection()))
		{
			try { if (!Connection->IO()) { disconnect(Connection); continue ; } }
			catch (std::exception &e) { log() << e.what() << std::endl; disconnect(Connection); continue ; }
			std::string line;
			if (!Connection->readLine(line))
				continue ;
			Message msg(line);
			BasicConnection *sender;
			if (!(sender = findSender(msg.prefix(), Connection)))
				{ disconnect(Connection); continue ; } //disconnect if msg of unknown origin
			exec(sender, msg);
		}
	}
}
