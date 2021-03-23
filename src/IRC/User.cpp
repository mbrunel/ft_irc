/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 17:11:05 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 23:24:50 by mbrunel          ###   ########.fr       */
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
_isRemote(false)
{}

User::~User()
{}

bool User::isAway() const
{
	return (_umode.isSet(UserMode::AWAY));
}

bool User::isRemote() const
{
	return (_isRemote);
}

const std::string &User::nickname() const
{
	return (_nickname);
}

const std::string &User::username() const
{
	return (_username);
}

const std::string &User::host() const
{
	return (_host);
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
