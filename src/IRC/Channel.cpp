/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:21:33 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/27 14:10:12 by asoursou         ###   ########.fr       */
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
_name(name), _type(UNKNOWN)
{
	if (name.size())
	{
		if (name[0] == '#')
			_type = GLOBAL;
		else if (name[0] == '&')
			_type = LOCAL;
	}
}

Channel::~Channel()
{}

void Channel::addMember(User *user, const MemberMode &mode)
{
	_members[user] = mode;
}

size_t Channel::count() const
{
	return (_members.size());
}

void Channel::delMember(User *user)
{
	_members.erase(user);
}

const MemberMode *Channel::findMember(User *user) const
{
	MemberMap::const_iterator i(_members.find(user));
	return (i != _members.end() ? &i->second : NULL);
}

const std::string &Channel::name() const
{
	return (_name);
}

Channel::Type Channel::type() const
{
	return (_type);
}

const MemberMap &Channel::members() const
{
	return (_members);
}

const ChannelMode &Channel::mode() const
{
	return (_mode);
}

const std::string &Channel::key() const
{
	return (_key);
}

const std::string &Channel::topic() const
{
	return (_topic);
}

void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

void Channel::setKey(const std::string &key)
{
	_key = key;
}
