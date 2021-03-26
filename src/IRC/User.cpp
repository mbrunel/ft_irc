/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 17:11:05 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 15:19:46 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

UserMode::UserMode(unsigned flags) :
Mode(flags)
{}

UserMode::~UserMode()
{}

User::User(TcpSocket *socket) :
BasicConnection(USER, socket),
_state(NEED_NICK),
_servertoken(0)
{}

User::~User()
{}

/*
RemoteServer *User::makeRemoteServer()
{
	TcpSocket *socket = _socket;
	_socket = NULL;
	return (new RemoteServer(socket));
}
*/

const std::string &User::nickname() const
{
	return (_nickname);
}

const std::string &User::username() const
{
	return (_username);
}

const UserMode &User::umode() const
{
	return (_umode);
}

const std::string &User::realname() const
{
	return (_realname);
}

const User::State &User::state() const
{
	return (_state);
}

void User::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

void User::setUsername(const std::string &username)
{
	_username = username;
}

void User::setRealname(const std::string &realname)
{
	_realname = realname;
}

void User::setState(const State &state)
{
	_state = state;
}
