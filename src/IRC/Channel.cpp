/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:21:33 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/24 18:04:01 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

ChannelMode::ChannelMode(unsigned flags) :
Mode(flags)
{}

ChannelMode::~ChannelMode()
{}

MemberMode::MemberMode(unsigned flags) :
Mode(flags)
{}

MemberMode::~MemberMode()
{}

Channel::Channel(const std::string &name) :
_name(name), _topic("An interesting topic"),
_isLocal(name.size() > 0 && name[0] == '&')
{}

Channel::~Channel()
{}

MemberMode &Channel::addUser(User *user)
{
	return (_members[user]);
}

void Channel::broadcast(User *user, const std::string &message)
{
	std::string buf;
	User		*u;

	buf = ':' + user->nickname() + ' ' + message + '\n';
	for (t_MemberMap::iterator i = _members.begin(); i != _members.end(); ++i)
		if (!(u = i->first)->isRemote())
			u->writeTo(buf);
}

size_t Channel::count() const
{
	return (_members.size());
}

void Channel::delUser(User *user)
{
	_members.erase(user);
}

const std::string &Channel::name() const
{
	return (_name);
}

const std::string &Channel::topic() const
{
	return (_topic);
}

const std::string &Channel::key() const
{
	return (_key);
}

const ChannelMode &Channel::mode() const
{
	return (_mode);
}

bool Channel::isLocal() const
{
	return (_isLocal);
}

const Channel::t_MemberMap &Channel::members() const
{
	return (_members);
}

void Channel::setName(const std::string &name)
{
	_name = name;
}

void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

void Channel::setKey(const std::string &key)
{
	_key = key;
}
