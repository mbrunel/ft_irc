/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:43:09 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 20:37:50 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "Mode.hpp"
#include "RemoteServer.hpp"
#include "Sender.hpp"

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

class User : public Sender
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

	bool				isAway() const;
	bool				isRemote() const;
	RemoteServer		*makeRemoteServer();
	TcpSocket			*socket() const;
	const std::string	&nickname() const;
	const std::string	&username() const;
	const std::string	&host() const;
	const UserMode		&umode() const;
	const std::string	&realname() const;
	const State			&state() const;
	void				setNickname(const std::string &nickname);
	void				setUsername(const std::string &username);
	void				setRealname(const std::string &realname);
	void				setState(const State &state);

protected:
	std::string	_nickname;
	std::string _username;
	std::string _host;
	UserMode	_umode;
	std::string _realname;
	unsigned	_servertoken;
	unsigned	_hopcount;
	State		_state;
	bool		_isRemote;

private:
	User(const User &other);

	User	&operator=(const User &other);
};
