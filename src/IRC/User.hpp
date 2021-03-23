/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:43:09 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 17:23:28 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "Mode.hpp"
#include "RemoteServer.hpp"
#include "TcpSocket.hpp"

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

class UserState : public Mode
{
public:
	enum State
	{
		NEED_PASS = 1,
		NEED_NICK = 1 << 2,
		NEED_USER = 1 << 3
	};

	UserState(unsigned flags = 0);
	virtual ~UserState();
};

class User
{
public:


	User(TcpSocket *socket);
	virtual ~User();

	bool				isAway() const;
	bool				isRemote() const;
	RemoteServer		*makeRemoteServer();
	TcpSocket			*socket() const;
	const std::string	&nickname() const;
	const std::string	&username() const;
	const std::string	&host() const;
	const UserMode		&umode() const;
	const std::string	&realname() const;
	const UserState		&ustate() const;
	void				setNickname(const std::string &nickname);
	void				setUsername(const std::string &username);

protected:
	TcpSocket	*_socket;
	std::string	_nickname;
	std::string _username;
	std::string _host;
	UserMode	_umode;
	std::string _realname;
	unsigned	_servertoken;
	unsigned	_hopcount;
	UserState	_ustate;
	bool		_isRemote;

private:
	User(const User &other);

	User	&operator=(const User &other);
};
