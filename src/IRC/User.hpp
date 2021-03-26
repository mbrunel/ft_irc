/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:43:09 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 15:20:06 by asoursou         ###   ########.fr       */
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
		AWAY = 1,
		INVISIBLE = 1 << 2,
		WALLOPS = 1 << 3,
		RESTRICTED = 1 << 4,
		OPERATOR = 1 << 5,
		LOCAL_OPERATOR = 1 << 6,
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
	const std::string	&host() const;
	const UserMode		&umode() const;
	const std::string	&realname() const;
	const State			&state() const;
	void				setNickname(const std::string &nickname);
	void				setUsername(const std::string &username);
	void				setRealname(const std::string &realname);
	void				setUmode(const UserMode &umode);
	void				setState(const State &state);

protected:
	std::string	_nickname;
	std::string _username;
	UserMode	_umode;
	std::string _realname;
	State		_state;
	unsigned	_servertoken;
};
