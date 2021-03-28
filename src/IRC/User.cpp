/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 17:11:05 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/28 19:58:09 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

UserMode::UserMode(unsigned flags) :
Mode(flags)
{}

UserMode::~UserMode()
{}

User::User(TcpSocket *socket) :
BasicConnection(socket, USER),
_state(HAS_NOTHING),
_joinedChannels(0)
{}

User::~User()
{}

const std::string &User::nickname() const
{
	return (_nickname);
}

const std::string &User::username() const
{
	return (_username);
}

const std::string &User::realname() const
{
	return (_realname);
}

const UserMode &User::umode() const
{
	return (_umode);
}

const User::State &User::state() const
{
	return (_state);
}

size_t User::joinedChannels() const
{
	return (_joinedChannels);
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

void User::setUmode(const UserMode &umode)
{
	_umode = umode;
}

void User::setState(const State &state)
{
	_state = state;
}

void User::setJoinedChannels(size_t joinedChannels)
{
	_joinedChannels = joinedChannels;
}
