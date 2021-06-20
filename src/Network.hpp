#pragma once
#include <assert.h>
#include <map>
#include <list>
#include <vector>
#include "Channel.hpp"
#include "Config.hpp"

struct UserInfo
{
	std::string nickname;
	std::string username;
	std::string host;
	std::string realname;
	UserInfo(const User &user);
};

class Network
{
  public:
	typedef std::map<tcp::TcpSocket *, BasicConnection *>	ConnectionMap;
	typedef std::map<std::string, User *>				UserMap;
	typedef std::map<std::string, User *>				ServiceMap;
	typedef std::map<std::string, Channel *>			ChannelMap;
	typedef std::map<std::string, Oper>					OperMap;
	typedef std::set<std::string>						FnicksSet;
	typedef std::list<BasicConnection *>				ZombieList;
	typedef std::vector<UserInfo>						InfoVec;
	typedef std::list<UserInfo>							HistoryList;

	Network();
	~Network() throw();

	const ChannelMap	&channels() const;
	const ConnectionMap	&connections() const;
	const UserMap		&users() const;
	const ServiceMap	&services() const;
	OperMap				&opers();
	FnicksSet			&fnicks();
	void				add(User *user);
	void				add(Channel *channel);
	void				addService(User *service);
	void				clear() throw();
	BasicConnection		*getBySocket(tcp::TcpSocket *socket);
	User				*getByNickname(const std::string &nickname);
	Channel				*getByChannelname(const std::string &channelname);
	User				*getByServicename(const std::string &servicename);
	void				msgToAll(const std::string &msg, BasicConnection *origin = NULL);
	void				remove(User *user) throw();
	void				remove(const Channel *chan) throw();
	void				removeService(User *user) throw();
	void				resetUserReceipt();
	Oper				*getOper(const std::string &login);
	bool				isFnick(const std::string &nick);
	void				addFnick(const std::string &nick);
	void				newZombie(BasicConnection *);
	BasicConnection		*nextZombie();
	void				addNickToHistory(const User &user);
	InfoVec				getNickHistory(const std::string &nick, size_t count);
	void				setHistorySize(size_t size);

  private:
	ConnectionMap	_connections;
	UserMap			_users;
	ServiceMap		_services;
	ChannelMap		_channels;
	OperMap			_opers;
	FnicksSet		_fnicks;
	ZombieList		_zombies;
	HistoryList		_history;
	size_t			_historySize;

	Network(const Network &);
	Network &operator=(const Network &);
};
