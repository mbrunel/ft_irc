/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:06:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/27 14:05:50 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include "User.hpp"

class ChannelMode : public Mode
{
public:
	enum Flag
	{
		/** Anonymous Channel flag */
		ANONYMOUS = 1,
		/** Invite-only Channel flag */
		INVITE_ONLY = 1 << 2,
		/** Moderated Channel flag */
		MODERATED = 1 << 3,
		/** No messages to Channel from User on the outside */
		NO_OUTSIDE_MSG = 1 << 4,
		/** Quiet channel flag */
		QUIET = 1 << 5,
		/** Private channel flag */
		PRIVATE = 1 << 6,
		/** Secret channel flag */
		SECRET = 1 << 7,
		/** Server reop Channel flag */
		REOP = 1 << 8,
		/** Topic settable by Channel operator only flag */
		TOPIC_SETTABLE_BY_CHANOP = 1 << 9
	};

	ChannelMode(unsigned flags = 0);
	virtual ~ChannelMode();
};

class MemberMode : public Mode
{
public:
	enum Flag
	{
		/** Channel creator status */
		CREATOR = 1,
		/** Channel operator privilege */
		OPERATOR = 1 << 2,
		/** Voice privilege */
		VOICE = 1 << 3
	};

	MemberMode(unsigned flags = 0);
	virtual ~MemberMode();
};

typedef std::map<User*, MemberMode>	MemberMap;

class Channel
{
public:
	enum Type
	{
		GLOBAL,
		LOCAL,
		UNKNOWN
	};

	Channel(const std::string &name = "");
	~Channel();

	void				addMember(User *user, const MemberMode &mode);
	size_t				count() const;
	void				delMember(User *user);
	const MemberMode	*findMember(User *user) const;
	const std::string	&name() const;
	Type				type() const;
	const MemberMap		&members() const;
	const ChannelMode	&mode() const;
	const std::string	&key() const;
	const std::string	&topic() const;
	void				setMode(const MemberMode &mode);
	void				setKey(const std::string &key);
	void				setTopic(const std::string &topic);

private:
	const std::string	_name;
	Type				_type;
	MemberMap			_members;
	ChannelMode			_mode;
	std::string			_key;
	std::string			_topic;

	//size_t					_limit;
	//std::list<std::string>	_banMasks;
	//std::list<std::string>	_exceptionMasks;
	//std::list<std::string>	_invitationMasks;
};
