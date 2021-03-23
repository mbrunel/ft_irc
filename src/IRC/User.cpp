/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 17:11:05 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 17:26:46 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

UserMode::UserMode(unsigned flags) :
Mode(flags)
{}

UserMode::~UserMode()
{}

UserState::UserState(unsigned flags) :
Mode(flags)
{}

UserState::~UserState()
{}

User::User(TcpSocket *socket) :
_socket(socket), _ustate(UserState::NEED_NICK | UserState::NEED_USER),
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

TcpSocket *User::socket() const
{
	return (_socket);
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

const UserState &User::ustate() const
{
	return (_ustate);
}

void User::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

void User::setUsername(const std::string &username)
{
	_username = username;
}
