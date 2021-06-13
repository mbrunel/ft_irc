#pragma once
#include <assert.h>
#include <map>
#include <list>
#include <vector>
#include "Channel.hpp"
#include "Config.hpp"

struct UserInfo
{
	std::string username;
	std::string host;
	std::string realname;
	UserInfo(const User &user);
};

class Network
{
  public:
	typedef std::map<TcpSocket *, BasicConnection *>	ConnectionMap;
	typedef std::map<std::string, User *>				UserMap;
	typedef std::map<std::string, Channel *>			ChannelMap;
	typedef std::map<std::string, Oper>					OperMap;
	typedef std::set<std::string>						FnicksSet;
	typedef std::list<BasicConnection *>				ZombieList;
	typedef std::vector<UserInfo>						HistoryVec;
	typedef std::map<std::string, HistoryVec>			HistoryVecMap;

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
	void				addNickToHistory(const User &user);
	HistoryVec			getNickHistory(const std::string &nick);

  private:
	ConnectionMap	_connections;
	UserMap			_users;
	ChannelMap		_channels;
	OperMap			_opers;
	FnicksSet		_fnicks;
	ZombieList		_zombies;
	HistoryVecMap	_history;

	Network(const Network &);
	Network &operator=(const Network &);
};
