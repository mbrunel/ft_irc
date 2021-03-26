/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:43:09 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 18:37:14 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
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
		INVISIBLE = 1 << 2,
		/** User receives wallops */
		WALLOPS = 1 << 3,
		/** Restricted User connection */
		RESTRICTED = 1 << 4,
		/** Operator flag */
		OPERATOR = 1 << 5,
		/** Local operator flag */
		LOCAL_OPERATOR = 1 << 6,
		/** Marks a User for receipt of server notices */
		MARK = 1 << 7
	};

	UserMode(unsigned flags = 0);
	virtual ~UserMode();
};

class User : public BasicConnection
{
public:
	enum State
	{
		NEED_PASS,
		NEED_NICK,
		NEED_USER,
		CONNECTED
	};

	User(TcpSocket *socket);
	virtual ~User();

	RemoteServer		*makeRemoteServer();
	const std::string	&nickname() const;
	const std::string	&username() const;
	const std::string	&realname() const;
	const UserMode		&umode() const;
	const State			&state() const;
	void				setNickname(const std::string &nickname);
	void				setUsername(const std::string &username);
	void				setRealname(const std::string &realname);
	void				setUmode(const UserMode &umode);
	void				setState(const State &state);

protected:
	std::string	_nickname;
	std::string _username;
	std::string _realname;
	UserMode	_umode;
	State		_state;
};
