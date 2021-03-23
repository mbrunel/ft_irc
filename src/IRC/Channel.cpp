/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:21:33 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 17:31:47 by asoursou         ###   ########.fr       */
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
_name(name)
{}

Channel::~Channel()
{}

MemberMode &Channel::addUser(User *user)
{
	t_MemberMap::iterator i = _members.find(user);

	if (i == _members.end())
	{
		_members[user] = MemberMode();
		i = _members.find(user);
	}
	return (i->second);
}

void Channel::broadcast(User *user, const std::string &message)
{
	std::string buf;
	User		*u;

	buf = ':' + user->nickname() + " PRIVMSG " + _name + " :" + message;
	for (t_MemberMap::iterator i = _members.begin(); i != _members.end(); ++i)
		if (!(u = i->first)->isRemote())
			u->socket()->writeLine(buf);
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

void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

void Channel::setKey(const std::string &key)
{
	_key = key;
}

bool Channel::isLocal() const
{
	return (_name.size() > 0 && _name[0] == '&');
}
