/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:06:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/24 15:55:40 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <list>
#include <map>
#include "User.hpp"

class MemberMode : public Mode
{
public:
	enum Flag
	{
		/** 'O' - Channel creator status */
		CREATOR = 1,
		/** 'o' - Channel operator privilege */
		OPERATOR = 1 << 2,
		/** 'v' - Voice privilege */
		VOICE = 1 << 3
	};

	MemberMode(unsigned flags = 0);
	~MemberMode();

	static Flag	parse(char c);
};

class ChannelMode : public Mode
{
public:
	enum Flag
	{
		/** 'a' - Anonymous Channel flag */
		ANONYMOUS = 1,
		/** 'i' - Invite-only Channel flag */
		INVITE_ONLY = 1 << 1,
		/** 'm' - Moderated Channel flag */
		MODERATED = 1 << 2,
		/** 'n' - No messages to Channel from User on the outside */
		NO_OUTSIDE_MSG = 1 << 3,
		/** 'q' - Quiet channel flag */
		QUIET = 1 << 4,
		/** 'p' - Private channel flag */
		PRIVATE = 1 << 5,
		/** 's' - Secret channel flag */
		SECRET = 1 << 6,
		/** 'r' - Server reop Channel flag */
		REOP = 1 << 7,
		/** 't' - Topic settable by Channel operator only flag */
		TOPIC_SETTABLE_BY_CHANOP = 1 << 8,
		/** 'k' - Channel key */
		KEY = 1 << 9,
		/** 'l' - User limit */
		LIMIT = 1 << 10,
		/** 'b' - Ban mask */
		BAN_MASK = 1 << 11,
		/** 'e' - Exception mask */
		EXCEPTION_MASK = 1 << 12,
		/** 'I' - Invitation mask */
		INVITATION_MASK = 1 << 13
	};

	ChannelMode(unsigned flags = 0);
	~ChannelMode();

	static Flag	parse(char c);

	std::string	toString() const;

private:
	/** Quick lookup table for lower alphabet */
	static const unsigned short _lowerFlagTable[26];
};

typedef std::map<User*, MemberMode>	MemberMap;

class Channel
{
public:
	typedef std::list<std::string> MasksList;

	enum Type
	{
		/** '#' */
		GLOBAL,
		/** '&' */
		LOCAL,
		/** '!' */
		SAFE,
		/** '+' */
		UNMODERATED
	};

	Channel(const std::string &name = "");
	~Channel();

	void				addMember(User *user, const MemberMode &mode);
	size_t				count() const;
	void				delMember(User *user);
	MemberMode			*findMember(User *user);
	bool				isLocal() const;
	void				send(const std::string &msg, BasicConnection *origin) const;
	const std::string	&name() const;
	Type				type() const;
	const MemberMap		&members() const;
	const ChannelMode	&mode() const;
	const std::string	&topic() const;
	const std::string	&key() const;
	size_t				limit() const;
	MasksList			&banMasks();
	MasksList			&exceptionMasks();
	MasksList			&invitationMasks();
	void				setMode(const ChannelMode &mode);
	void				setTopic(const std::string &topic);
	void				setKey(const std::string &key);
	void				setLimit(size_t limit);

private:
	const std::string	_name;
	Type				_type;
	MemberMap			_members;
	ChannelMode			_mode;
	std::string			_topic;
	std::string			_key;
	size_t				_limit;
	MasksList			_banMasks;
	MasksList			_exceptionMasks;
	MasksList			_invitationMasks;
};
