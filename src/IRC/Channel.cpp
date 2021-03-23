/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:21:33 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 15:09:37 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Membership::Membership()
{}

Membership::~Membership()
{}

Channel::Channel(const std::string &name) :
_name(name)
{}

Channel::~Channel()
{}

Membership &Channel::addUser(User *user)
{
	t_MembershipMap::iterator i = _members.find(user);

	if (i == _members.end())
	{
		_members[user] = Membership();
		i = _members.find(user);
	}
	return (i->second);
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
