#include "Network.hpp"

UserInfo::UserInfo(const User &u):
	nickname(u.nickname()),
	username(u.username()),
	host(u.socket()->host()),
	realname(u.realname()) {}

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

const Network::UserMap &Network::users() const
{
	return (_users);
}

const Network::ServiceMap &Network::services() const
{
	return (_services);
}

void Network::add(User *u)
{
	_connections[u->socket()] = u;
	if (u->nickname().size())
		_users[u->nickname()] = u;
}

void Network::add(Channel *c)
{
	_channels[c->name()] = c;
}

void Network::addService(User *u)
{
	_connections[u->socket()] = u;
	_services[u->nickname()] = u;
}

void Network::clear() throw()
{
	for (UserMap::iterator i = _users.begin(); i != _users.end(); ++i)
		delete i->second;
	for (ChannelMap::iterator i = _channels.begin(); i != _channels.end(); ++i)
		delete i->second;
	for (ServiceMap::iterator i = _services.begin(); i != _services.end(); ++i)
		delete i->second;
	for (ZombieList::iterator i = _zombies.begin(); i != _zombies.end(); ++i)
		delete *i;
}

BasicConnection *Network::getBySocket(tcp::TcpSocket *socket)
{
	ConnectionMap::const_iterator i = _connections.find(socket);
	return (i == _connections.end() ? NULL : i->second);
}

User *Network::getByNickname(const std::string &key)
{
	UserMap::const_iterator i = _users.find(key);
	return (i == _users.end() ? NULL : i->second);
}

Channel *Network::getByChannelname(const std::string &key)
{
	ChannelMap::const_iterator i = _channels.find(key);
	return (i == _channels.end() ? NULL : i->second);
}

User *Network::getByServicename(const std::string &key)
{
	ServiceMap::const_iterator i = _services.find(key);
	return (i == _services.end() ? NULL : i->second);
}

void Network::remove(User *u) throw()
{
	if (!u->hopcount())
		_connections.erase(u->socket());
	_users.erase(u->nickname());
}

void Network::remove(const Channel *c) throw()
{
	_channels.erase(c->name());
}

void Network::removeService(User *u) throw()
{
	if (!u->hopcount())
		_connections.erase(u->socket());
	_services.erase(u->nickname());
}

void Network::msgToAll(const std::string &msg, BasicConnection *origin)
{
	for (UserMap::const_iterator i = _users.begin(); i != _users.end(); ++i)
	{
		User *u(i->second);
		if (!u->hopcount() && u->isRegistered() && u->socket() != origin->socket())
			u->writeLine(msg);
	}
}

void Network::resetUserReceipt()
{
	for (UserMap::const_iterator i = _users.begin(); i != _users.end(); ++i)
		i->second->unmark();
}

Network::OperMap &Network::opers() { return _opers; }

Oper *Network::getOper(const std::string &login)
{
	OperMap::iterator it = _opers.find(login);
	if (it == _opers.end())
		return (NULL);
	return &it->second;
}

Network::FnicksSet &Network::fnicks() {return _fnicks; }

bool Network::isFnick(const std::string &nick)
{
	FnicksSet::iterator it = _fnicks.find(nick);
	return (it == _fnicks.end() ? false : true);
}

void Network::addFnick(const std::string &nick)
{
	_fnicks.insert(nick);
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

void Network::addNickToHistory(const User &u)
{
	if (_historySize == _history.size())
		_history.pop_front();
	_history.push_back(UserInfo(u));
}

Network::InfoVec Network::getNickHistory(const std::string &nick, size_t count)
{
	InfoVec v;

	if (!count)
		count = -1;
	for (HistoryList::reverse_iterator rit = _history.rbegin(); rit != _history.rend() && count; ++rit)
		if (rit->nickname == nick)
		{
			--count;
			v.push_back(*rit);
		}
	return v;

}

void Network::setHistorySize(size_t size){ _historySize = size; }
