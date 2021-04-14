/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 17:11:05 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/14 15:28:28 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageBuilder.hpp"
#include "User.hpp"

UserRequirement::UserRequirement(unsigned flags) :
Mode(flags)
{}

UserRequirement::~UserRequirement()
{}

UserMode::UserMode(unsigned flags) :
Mode(flags)
{}

UserMode::~UserMode()
{}

User::User(TcpSocket *socket, UserRequirement::Flag requirements) :
BasicConnection(socket, USER),
_requirements(requirements),
_joinedChannels(0)
{}

User::~User()
{}

bool User::isRegistered() const
{
	return (!_requirements.flags());
}

RemoteServer *User::makeRemoteServer(int hopcount)
{
	//return (new RemoteServer(_socket, hopcount));
	(void)hopcount;
	return (NULL);
}

void User::unsetRequirement(UserRequirement::Flag requirement)
{
	_requirements.unset(requirement);
}

const UserRequirement &User::requirements() const
{
	return (_requirements);
}

const std::string &User::realname() const
{
	return (_realname);
}

const UserMode &User::umode() const
{
	return (_umode);
}

size_t User::joinedChannels() const
{
	return (_joinedChannels);
}

const std::string &User::prefix() const
{
	return (_prefix);
}

const std::string &User::nickname() const
{
	return (_nickname);
}

const std::string &User::username() const
{
	return (_username);
}

void User::setRealname(const std::string &realname)
{
	_realname = realname;
}

void User::setUmode(const UserMode &umode)
{
	_umode = umode;
}

void User::setJoinedChannels(size_t joinedChannels)
{
	_joinedChannels = joinedChannels;
}

void User::setNickname(const std::string &nickname)
{
	_nickname = nickname;
	updatePrefix();
}

void User::setUsername(const std::string &username)
{
	_username = username;
	updatePrefix();
}

void User::updatePrefix()
{
	_prefix = nickname();
	if (_username.size())
		_prefix += "!~" + username();
	_prefix += "@" + _socket->host();
}
