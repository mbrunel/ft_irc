/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 22:26:27 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

IrcServer::IrcServer() {}

IrcServer::~IrcServer() {}

void IrcServer::setMaxConnections(size_t MaxConnections) { srv.setMaxConnections(MaxConnections); }

void IrcServer::setVerbose(bool verbose) { srv.setVerbose(verbose); }

void IrcServer::setLogDestination(const std::string &destfile) { srv.setLogDestination(destfile); }

std::ostream &IrcServer::log() throw() { return srv.log(); }

void IrcServer::listen(const char *port, SSL_CTX *ctx, size_t maxQueueLen) { srv.listen(port, ctx, maxQueueLen); }

Sender *IrcServer::findSender(const Prefix &prefix, TcpSocket *Connection)
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

void IrcServer::exec(Sender *sender, const Message &msg)
{
	cmdMapType::iterator it;

	if ((it = commands.find(cmdIdType(msg.command(), sender->type()))) == commands.end())
		sender->writeTo("Unknown Command\n");
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
			newConnection->writeLine("Connection established");
			localUsers[newConnection] = new User(newConnection);
		}
		TcpSocket *Connection;
		while ((Connection = srv.nextPendingConnection()))
		{
			try { if (!Connection->IO()) { srv.disconnect(Connection); continue ; } }
			catch (std::exception &e) { log() << e.what() << std::endl; srv.disconnect(Connection); continue ; }
			std::string line;
			if (!Connection->readLine(line))
				continue ;
			Message msg(line);
			Sender *sender;
			if (!(sender = findSender(msg.prefix(), Connection)))
				{ srv.disconnect(Connection); continue ; } //disconnect if unknown connection sent a msg
			exec(sender, msg);
		}
	}
}
