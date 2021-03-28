/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 00:47:13 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/28 21:50:27 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Network.hpp"

Network::Network() {}

Network::~Network() throw() { clear(); }

User *Network::newUser(BasicConnection *connection)
{
	User *usr = new User(connection->socket());
	addUser(usr);
	addConnection(usr);
	return usr;
}

RemoteServer *Network::newServer(BasicConnection *connection)
{
	RemoteServer *srv = new RemoteServer(connection->socket());
	addServer(srv);
	addConnection(srv);
	return srv;
}

BasicConnection *Network::newConnection(TcpSocket *socket)
{
	BasicConnection *connection = new BasicConnection(socket);
	addConnection(connection);
	return (connection);
}

Channel *Network::newChan(const std::string &name)
{
	Channel *chan = new Channel(name);
	addChan(chan);
	return chan;
}

void Network::addUser(User *user)
{
	users[user->nickname()] = user;
}

void Network::addServer(RemoteServer *srv)
{
	servers[srv->name()] = srv;
}

void Network::addConnection(BasicConnection *connection)
{
	connections[connection->socket()] = connection;
}

void Network::addChan(Channel *chan)
{
	channels[chan->name()] = chan;
}

bool Network::isUser(const std::string &nick)
{
	if (users.find(nick) == users.end())
		return false;
	return true;
}

bool Network::isServer(const std::string &name)
{
	if (servers.find(name) == servers.end())
		return false;
	return true;
}

bool Network::isChan(const std::string &name)
{
	if (channels.find(name) == channels.end())
		return false;
	return true;
}

User *Network::getUser(const std::string &nick)
{
	UserMap::iterator it = users.find(nick);
	if (it == users.end())
		return NULL;
	return it->second;
}

Channel *Network::getChan(const std::string &name)
{
	ChannelMap::iterator it = channels.find(name);
	if (it == channels.end())
		return NULL;
	return it->second;
}

BasicConnection *Network::getConnection(TcpSocket *socket)
{
	LocalMap::iterator it = connections.find(socket);
	if (it == connections.end())
		return NULL;
	return it->second;
}

RemoteServer *Network::getServer(const std::string &srvname)
{
	ServerMap::iterator it = servers.find(srvname);
	if (it == servers.end())
		return NULL;
	return it->second;
}

void Network::removeUser(const std::string &nick) throw()
{
	users.erase(nick);
}

void Network::removeUser(User *user) throw()
{
	removeUser(user->nickname());
}

void Network::removeServer(const std::string &srvname) throw()
{
	servers.erase(srvname);
}

void Network::removeServer(RemoteServer *srv) throw()
{
	removeUser(srv->name());
}

void Network::removeConnection(TcpSocket *socket) throw()
{
	connections.erase(socket);
}

void Network::removeConnection(BasicConnection *connection) throw()
{
	removeConnection(connection->socket());
}

void Network::removeChan(const std::string &name) throw()
{
	channels.erase(name);
}

void Network::removeChan(const Channel *chan) throw()
{
	removeChan(chan->name());
}

void Network::msgToNetwork(const std::string &msg)
{
	for (ServerMap::iterator it = servers.begin(); it != servers.end(); it++)
		it->second->writeLine(msg);
}

void Network::msgToChan(Channel *chan, const std::string &msg, User *sender)
{
	const MemberMap &map(chan->members());

	for (MemberMap::const_iterator i = map.begin(); i != map.end(); ++i)
	{
		User *user = i->first;
		if (!user->hopcount() && user != sender)
			user->writeLine(msg);
	}
	msgToNetwork(msg);
}

User *Network::conToUsr(BasicConnection *connection)
{
	User *usr = connection->type() == BasicConnection::USER ? static_cast<User *>(connection) : NULL;
	assert(usr);
	return usr;
}

RemoteServer *Network::conToSrv(BasicConnection *connection)
{
	return connection->type() == BasicConnection::SERVER ? static_cast<RemoteServer *>(connection) : NULL;
}

void Network::clear() throw()
{
	for (LocalMap::iterator it = connections.begin(); it != connections.end(); it++)
		delete it->second;
	for (UserMap::iterator it = users.begin(); it != users.end(); it++)
		if (it->second->hopcount())
			delete it->second;
	for (ServerMap::iterator it = servers.begin(); it != servers.end(); it++)
		delete it->second;
	for (ChannelMap::iterator it = channels.begin(); it != channels.end(); it++)
		delete it->second;
}
