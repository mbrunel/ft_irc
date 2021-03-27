/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/27 15:44:21 by asoursou         ###   ########.fr       */
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
	load("PRIVMSG", BasicConnection::USER, &IrcServer::privmsg);
	load("TOPIC", BasicConnection::USER, &IrcServer::topic);
	load("USER", BasicConnection::USER, &IrcServer::userUser);
}

IrcServer::~IrcServer()
{
	for (nickIter it = allUsers. begin() ; it != allUsers.end() ; ++it)
		if (it->second->hopcount())
			delete it->second;
	for (sockIter it = localUsers.begin() ; it != localUsers.end() ; ++it)
		disconnect(it->second);
}

void IrcServer::setMaxConnections(size_t MaxConnections) { srv.setMaxConnections(MaxConnections); }

void IrcServer::setVerbose(bool verbose) { srv.setVerbose(verbose); }

void IrcServer::setLogDestination(const std::string &destfile) { srv.setLogDestination(destfile); }

std::ostream &IrcServer::log() throw() { return srv.log(); }

void IrcServer::listen(const char *port, SSL_CTX *ctx, size_t maxQueueLen) { srv.listen(port, ctx, maxQueueLen); }


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
			try { if (!Connection->IO()) { disconnect(Connection); continue ; } }
			catch (std::exception &e) { log() << e.what() << std::endl; disconnect(Connection); continue ; }
			std::string line;
			if (!Connection->readLine(line))
				continue ;
			Message msg(line);
			if (!msg.isValid())
				continue ;
			log() << msg << std::endl;
			BasicConnection *sender;
			if (!(sender = findSender(msg.prefix(), Connection)))
				{ disconnect(Connection); continue ; } //disconnect if msg of unknown origin
			exec(sender, msg);
		}
	}
}

void IrcServer::broadcast(const Channel &channel, const std::string &message, User *except)
{
	const MemberMap	&map(channel.members());
	User			*user;

	for (MemberMap::const_iterator i = map.begin(); i != map.end(); ++i)
		if ((user = i->first) != except && !user->hopcount())
			user->writeLine(message);
	// Add broadcast to all servers
}

void IrcServer::load(const std::string &cmd, BasicConnection::Type type, cmdType handler)
{
	commands[cmdIdType(cmd, type)] = handler;
}

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
		sender->writeLine("Unknown Command");
		return ;
	}
	(this->*(it->second))(sender, msg);
}
