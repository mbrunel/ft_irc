#pragma once
#include "BasicConnection.hpp"
#include "Mode.hpp"

class UserMode : public Mode
{
public:
	enum Flag
	{
		/** 'a' - User is flagged as away */
		AWAY = 1,
		/** 'i' - Marks a User as invisible */
		INVISIBLE = 1 << 1,
		/** 'w' - User receives wallops */
		WALLOPS = 1 << 2,
		/** 'r' - Restricted User connection */
		RESTRICTED = 1 << 3,
		/** 'o' - Operator flag */
		OPERATOR = 1 << 4,
		/** 's' - Marks a User for receipt of server notices */
		MARK = 1 << 6
	};

	UserMode(unsigned flags = 0);
	~UserMode();

	static Flag	parse(char c);

	std::string	toString() const;

private:
	/** Quick lookup table for lower alphabet */
	static const unsigned short _lowerFlagTable[26];
};

class UserRequirement : public Mode
{
public:
	enum Flag
	{
		PASS = 1,
		NICK = 1 << 1,
		USER = 1 << 2,
		ALL_EXCEPT_PASS = NICK | USER,
		ALL = PASS | ALL_EXCEPT_PASS
	};

	UserRequirement(unsigned flags = 0);
	virtual ~UserRequirement();
};

class User : public BasicConnection
{
public:
	User(tcp::TcpSocket *socket, UserRequirement::Flag requirements);
	virtual ~User();

	bool					isRegistered() const;
	void					mark();
	time_t					&idle();
	void					unmark();
	void					setRequirement(UserRequirement::Flag requirement);
	void					unsetRequirement(UserRequirement::Flag requirement);
	const UserRequirement	&requirements() const;
	const std::string		&realname() const;
	const UserMode			&umode() const;
	size_t					joinedChannels() const;
	const std::string		&awayReason() const;
	const std::string		&prefix() const;
	const std::string		&nickname() const;
	const std::string		&username() const;
	void					setRealname(const std::string &realname);
	void					setUmode(const UserMode &umode);
	void					setJoinedChannels(size_t joinedChannels);
	void					setAway(const std::string &reason);
	void					setNickname(const std::string &nickname);
	void					setUsername(const std::string &username);

protected:
	UserRequirement	_requirements;
	std::string		_realname;
	UserMode		_umode;
	size_t			_joinedChannels;
	std::string		_awayReason;
	time_t			_idle;

private:
	std::string		_prefix;
	std::string		_nickname;
	std::string		_username;

	void	updatePrefix();
};
