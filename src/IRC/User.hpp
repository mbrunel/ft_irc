/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:43:09 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/20 16:37:38 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "IrcNumeric.hpp"
#include "Mode.hpp"
#include "RemoteServer.hpp"
#include "BasicConnection.hpp"

class UserMode : public Mode
{
public:
	enum Flag
	{
		/** User is flagged as away */
		AWAY = 1,
		/** Marks a User as invisible */
		INVISIBLE = 1 << 1,
		/** User receives wallops */
		WALLOPS = 1 << 2,
		/** Restricted User connection */
		RESTRICTED = 1 << 3,
		/** Operator flag */
		OPERATOR = 1 << 4,
		/** Local operator flag */
		LOCAL_OPERATOR = 1 << 5,
		/** Marks a User for receipt of server notices */
		MARK = 1 << 6
	};

	UserMode(unsigned flags = 0);
	virtual ~UserMode();
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
	User(TcpSocket *socket, UserRequirement::Flag requirements);
	virtual ~User();

	bool					isRegistered() const;
	RemoteServer			*makeRemoteServer(int hopcount);
	void					unsetRequirement(UserRequirement::Flag requirement);
	const UserRequirement	&requirements() const;
	const std::string		&realname() const;
	const UserMode			&umode() const;
	const std::string		&prefix() const;
	const std::string		&nickname() const;
	const std::string		&username() const;
	size_t					joinedChannels() const;
	void					setRealname(const std::string &realname);
	void					setUmode(const UserMode &umode);
	void					setJoinedChannels(size_t joinedChannels);
	void					setNickname(const std::string &nickname);
	void					setUsername(const std::string &username);

protected:
	UserRequirement	_requirements;
	std::string		_realname;
	UserMode		_umode;
	size_t			_joinedChannels;

private:
	std::string		_prefix;
	std::string		_nickname;
	std::string		_username;
	

	void	updatePrefix();
};
