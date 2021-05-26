/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 14:21:33 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/26 16:15:12 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include "Channel.hpp"

MemberMode::MemberMode(unsigned flags) :
Mode(flags)
{}

MemberMode::~MemberMode()
{}

MemberMode::Flag MemberMode::parse(char c)
{
	if (c == 'O')
		return (CREATOR);
	if (c == 'o')
		return (OPERATOR);
	return (static_cast<Flag>((c == 'v') * VOICE));
}

ChannelMode::ChannelMode(unsigned flags) :
Mode(flags)
{}

ChannelMode::~ChannelMode()
{}

ChannelMode::Flag ChannelMode::parse(char c)
{
	unsigned f;
	f = islower(c) ? _lowerFlagTable[c - 'a'] : (c == 'I') * INVITATION_MASK;
	return (static_cast<Flag>(f));
}

std::string ChannelMode::toString() const
{
	std::string s;
	if (isSet(ANONYMOUS))
		s.push_back('a');
	if (isSet(INVITE_ONLY))
		s.push_back('i');
	if (isSet(NO_OUTSIDE_MSG))
		s.push_back('n');
	if (isSet(QUIET))
		s.push_back('q');
	if (isSet(PRIVATE))
		s.push_back('p');
	if (isSet(SECRET))
		s.push_back('s');
	if (isSet(REOP))
		s.push_back('r');
	if (isSet(TOPIC_SETTABLE_BY_CHANOP))
		s.push_back('t');
	if (isSet(KEY))
		s.push_back('k');
	if (isSet(LIMIT))
		s.push_back('l');
	return (s);
}

const unsigned short ChannelMode::_lowerFlagTable[] =
{
	ANONYMOUS, BAN_MASK, 0, 0, EXCEPTION_MASK, 0, 0, 0, INVITE_ONLY, 0, KEY,
	LIMIT, MODERATED, NO_OUTSIDE_MSG, 0, PRIVATE, QUIET, REOP, SECRET,
	TOPIC_SETTABLE_BY_CHANOP, 0, 0, 0, 0, 0, 0
};

Channel::Channel(const std::string &name) :
_name(name), _type(GLOBAL)
{
	const char *types = "#&!+";
	if (name.size())
		_type = static_cast<Type>(strchrnul(types, name[0]) - types);
}

Channel::~Channel()
{}

void Channel::addMember(User *user, const MemberMode &mode)
{
	_members[user] = mode;
	user->setJoinedChannels(user->joinedChannels() + 1);
	if (isInvited(user))
		_invitations.erase(_invitations.find(user->nickname()));
}

size_t Channel::count() const
{
	return (_members.size());
}

void Channel::delMember(User *user)
{
	_members.erase(user);
	user->setJoinedChannels(user->joinedChannels() - 1);
}

MemberMode *Channel::findMember(User *user)
{
	MemberMap::iterator i(_members.find(user));
	return (i != _members.end() ? &i->second : NULL);
}

void Channel::invite(User *user)
{
	_invitations.insert(user->nickname());
}

bool Channel::isInvited(User *user) const
{
	return (_invitations.find(user->nickname()) != _invitations.end());
}

bool Channel::isLocal() const
{
	return (_type == LOCAL);
}

void Channel::send(const std::string &msg, BasicConnection *origin) const
{
	for (MemberMap::const_iterator i = _members.begin(); i != _members.end(); ++i)
	{
		User *user(i->first);
		if (!user->hopcount() && (!origin || user->socket() != origin->socket()))
			user->writeLine(msg);
	}
}

const std::string &Channel::name() const
{
	return (_name);
}

Channel::Type Channel::type() const
{
	return (_type);
}

const Channel::MemberMap &Channel::members() const
{
	return (_members);
}

const ChannelMode &Channel::mode() const
{
	return (_mode);
}

const std::string &Channel::topic() const
{
	return (_topic);
}

const std::string &Channel::key() const
{
	return (_key);
}

size_t Channel::limit() const
{
	return (_limit);
}

Channel::MaskSet &Channel::banMasks()
{
	return (_banMasks);
}

Channel::MaskSet &Channel::exceptionMasks()
{
	return (_exceptionMasks);
}

Channel::MaskSet &Channel::invitationMasks()
{
	return (_invitationMasks);
}

void Channel::setMode(const ChannelMode &mode)
{
	_mode = mode;
}

void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

void Channel::setKey(const std::string &key)
{
	_key = key;
}

void Channel::setLimit(size_t limit)
{
	_limit = limit;
}
