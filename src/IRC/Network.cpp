/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 00:47:13 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/14 13:49:53 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Network.hpp"

Network::Network() {}

Network::~Network() throw()
{
	clear();
}

void Network::add(User *u)
{
	connections[u->socket()] = u;
	if (u->nickname().size())
		users[u->nickname()] = u;
}

void Network::add(Server *s)
{
	connections[s->socket()] = s;
	servers[s->name()] = s;
}

void Network::add(Channel *c)
{
	channels[c->name()] = c;
}

void Network::clear() throw()
{
	for (UserMap::iterator i = users.begin(); i != users.end(); ++i)
		if (i->second->hopcount())
			delete i->second;
	for (ServerMap::iterator i = servers.begin(); i != servers.end(); ++i)
		delete i->second;
	for (ChannelMap::iterator i = channels.begin(); i != channels.end(); ++i)
		delete i->second;
}

BasicConnection *Network::getBySocket(TcpSocket *socket)
{
	ConnectionMap::const_iterator i = connections.find(socket);
	return (i == connections.end() ? NULL : i->second);
}

User *Network::getByNickname(const std::string &key)
{
	UserMap::const_iterator i = users.find(key);
	return (i == users.end() ? NULL : i->second);
}

Server *Network::getByServername(const std::string &key)
{
	ServerMap::const_iterator i = servers.find(key);
	return (i == servers.end() ? NULL : i->second);
}

Channel *Network::getByChannelname(const std::string &key)
{
	ChannelMap::const_iterator i = channels.find(key);
	return (i == channels.end() ? NULL : i->second);
}

void Network::remove(User *u) throw()
{
	if (u->hopcount())
		connections.erase(u->socket());
	users.erase(u->nickname());
}

void Network::remove(Server *s) throw()
{
	connections.erase(s->socket());
	servers.erase(s->name());
}

void Network::remove(const Channel *c) throw()
{
	channels.erase(c->name());
}

void Network::msgToAll(const std::string &msg, BasicConnection *origin)
{
	for (UserMap::const_iterator i = users.begin(); i != users.end(); ++i)
	{
		User *u(i->second);
		if (!u->hopcount() && u->isRegistered() && u->socket() != origin->socket())
			u->writeLine(msg);
	}
	msgToNetwork(msg, origin);
}

void Network::msgToChan(const Channel *channel, const std::string &msg, BasicConnection *origin)
{
	channel->send(msg, origin);
	if (!channel->isLocal())
		msgToNetwork(msg, origin);
}

void Network::msgToNetwork(const std::string &msg, BasicConnection *origin)
{
	for (ServerMap::const_iterator i = servers.begin(); i != servers.end(); ++i)
	{
		Server *server(i->second);
		if (!server->hopcount() && (!origin || server->socket() != origin->socket()))
			server->writeLine(msg);
	}
}
