#pragma once
#include <assert.h>
#include <map>
#include <list>
#include "Channel.hpp"
#include "Config.hpp"

class Network
{
  public:
	typedef std::map<TcpSocket *, BasicConnection *>	ConnectionMap;
	typedef std::map<std::string, User *>				UserMap;
	typedef std::map<std::string, Channel *>			ChannelMap;
	typedef std::map<std::string, Oper>					OperMap;
	typedef std::set<std::string>						FnicksSet;
	typedef std::list<BasicConnection *>				ZombieList;

	Network();
	~Network() throw();

	const ChannelMap	&channels() const;
	const ConnectionMap	&connections() const;
	const UserMap		&users() const;
	OperMap				&opers();
	FnicksSet			&fnicks();
	void				add(User *user);
	void				add(Channel *channel);
	void				clear() throw();
	BasicConnection		*getBySocket(TcpSocket *socket);
	User				*getByNickname(const std::string &nickname);
	Channel				*getByChannelname(const std::string &channelname);
	void				msgToAll(const std::string &msg, BasicConnection *origin = NULL);
	void				remove(User *user) throw();
	void				remove(const Channel *chan) throw();
	void				resetUserReceipt();
	Oper				*getOper(const std::string &login);
	bool				isFnick(const std::string &nick);
	void				addFnick(const std::string &nick);
	void				newZombie(BasicConnection *);
	BasicConnection		*nextZombie();

  private:
	ConnectionMap	_connections;
	UserMap			_users;
	ChannelMap		_channels;
	OperMap			_opers;
	FnicksSet		_fnicks;
	ZombieList		_zombies;

	Network(const Network &);
	Network &operator=(const Network &);
};
