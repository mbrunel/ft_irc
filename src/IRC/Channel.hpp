/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:06:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 18:43:03 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include "Mode.hpp"
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

class Channel
{
public:
	typedef std::map<User*, MemberMode>	t_MemberMap;

	Channel(const std::string &name = "");
	virtual ~Channel();

	MemberMode			&addUser(User *user);
	size_t				count() const;
	void				delUser(User *user);
	const std::string	&name() const;
	const std::string	&topic() const;
	const std::string	&key() const;
	const ChannelMode	&mode() const;
	bool				isLocal() const;
	const t_MemberMap	&members() const;
	void				setName(const std::string &name);
	void				setTopic(const std::string &topic);
	void				setKey(const std::string &key);

private:
	std::string	_name;
	std::string	_topic;
	std::string	_key;
	ChannelMode	_mode;
	bool		_isLocal;
	t_MemberMap	_members;

	//size_t					_limit;
	//std::list<std::string>	_banMasks;
	//std::list<std::string>	_exceptionMasks;
	//std::list<std::string>	_invitationMasks;
};
