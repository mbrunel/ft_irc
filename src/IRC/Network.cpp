/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 00:47:13 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/01 22:22:29 by mbrunel          ###   ########.fr       */
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
	if (u->isRegistered())
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
	ConnectionMap::iterator i = connections.find(socket);
	return (i == connections.end() ? NULL : i->second);
}

User *Network::getByNickname(const std::string &key)
{
	UserMap::iterator i = users.find(key);
	return (i == users.end() ? NULL : i->second);
}

Server *Network::getByServername(const std::string &key)
{
	ServerMap::iterator i = servers.find(key);
	return (i == servers.end() ? NULL : i->second);
}

Channel *Network::getByChannelname(const std::string &key)
{
	ChannelMap::iterator i = channels.find(key);
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

void Network::msgToNetwork(const std::string &msg, BasicConnection *sender)
{
	for (ServerMap::iterator it = servers.begin(); it != servers.end(); ++it)
		if (!it->second->hopcount() && it->second->socket() != sender->socket())
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
	msgToNetwork(msg, sender);
}
