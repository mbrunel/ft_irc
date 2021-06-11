#include "Network.hpp"

Network::Network() {}

Network::~Network() throw()
{
	clear();
}


const Network::ChannelMap &Network::channels() const
{
	return (_channels);
}
const Network::ConnectionMap &Network::connections() const
{
	return (_connections);
}
const Network::ServerMap &Network::servers() const
{
	return (_servers);
}
const Network::UserMap &Network::users() const
{
	return (_users);
}
void Network::add(User *u)
{
	_connections[u->socket()] = u;
	if (u->nickname().size())
		_users[u->nickname()] = u;
}

void Network::add(Server *s)
{
	_connections[s->socket()] = s;
	_servers[s->name()] = s;
}

void Network::add(Channel *c)
{
	_channels[c->name()] = c;
}

void Network::clear() throw()
{
	for (UserMap::iterator i = _users.begin(); i != _users.end(); ++i)
		if (i->second->hopcount())
			delete i->second;
	for (ServerMap::iterator i = _servers.begin(); i != _servers.end(); ++i)
		delete i->second;
	for (ChannelMap::iterator i = _channels.begin(); i != _channels.end(); ++i)
		delete i->second;
}

BasicConnection *Network::getBySocket(TcpSocket *socket)
{
	ConnectionMap::const_iterator i = _connections.find(socket);
	return (i == _connections.end() ? NULL : i->second);
}

User *Network::getByNickname(const std::string &key)
{
	UserMap::const_iterator i = _users.find(key);
	return (i == _users.end() ? NULL : i->second);
}

Server *Network::getByServername(const std::string &key)
{
	ServerMap::const_iterator i = _servers.find(key);
	return (i == _servers.end() ? NULL : i->second);
}

Channel *Network::getByChannelname(const std::string &key)
{
	ChannelMap::const_iterator i = _channels.find(key);
	return (i == _channels.end() ? NULL : i->second);
}

void Network::remove(User *u) throw()
{
	if (!u->hopcount())
		_connections.erase(u->socket());
	_users.erase(u->nickname());
}

void Network::remove(Server *s) throw()
{
	_connections.erase(s->socket());
	_servers.erase(s->name());
}

void Network::remove(const Channel *c) throw()
{
	_channels.erase(c->name());
}

void Network::msgToAll(const std::string &msg, BasicConnection *origin)
{
	for (UserMap::const_iterator i = _users.begin(); i != _users.end(); ++i)
	{
		User *u(i->second);
		if (!u->hopcount() && u->isRegistered() && u->socket() != origin->socket())
			u->writeLine(msg);
	}
	msgToNetwork(msg, origin);
}

void Network::msgToChan(const Channel *channel, const std::string &msg, BasicConnection *origin, bool useReceipt)
{
	channel->send(msg, origin, useReceipt);
	if (!channel->isLocal())
		msgToNetwork(msg, origin);
}

void Network::msgToNetwork(const std::string &msg, BasicConnection *origin)
{
	for (ServerMap::const_iterator i = _servers.begin(); i != _servers.end(); ++i)
	{
		Server *server(i->second);
		if (!server->hopcount() && (!origin || server->socket() != origin->socket()))
			server->writeLine(msg);
	}
}

void Network::resetUserReceipt()
{
	for (UserMap::const_iterator i = _users.begin(); i != _users.end(); ++i)
		i->second->unmark();
}

void Network::setOpers(OperMap o) { _opers = o; }

Oper *Network::getOper(std::string login)
{
	OperMap::iterator it = _opers.find(login);
	if (it == _opers.end())
		return (NULL);
	return &it->second;
}

void Network::newZombie(BasicConnection *z)
{
	_zombies.push_back(z);
}

BasicConnection *Network::nextZombie()
{
	if (_zombies.empty())
		return NULL;
	BasicConnection *z = _zombies.front();
	_zombies.pop_front();
	return z;
}
