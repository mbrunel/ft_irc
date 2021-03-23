/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:06:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 13:46:13 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <list>
#include <map>
#include <string>

class ChannelMode
{
public:
	enum Flag
	{
		ANONYMOUS = 1,
		INVITE_ONLY = 1 << 2,
		MODERATED = 1 << 3,
		NO_OUTSIDE_MSG = 1 << 4,
		QUIET = 1 << 5,
		PRIVATE = 1 << 6,
		SECRET = 1 << 7,
		REOP = 1 << 8,
		TOPIC_SETTABLE_BY_CHANOP = 1 << 9
	};

	ChannelMode(const Flag flag = 0);
	~ChannelMode();

	const Flag	isSet(const Flag flag) const;
	void		setFlag(const Flag flag);
	void		unsetFlag(const Flag flag);

private:
	unsigned	_flags;
};

class MembershipMode
{
public:
	enum Flag
	{
		CREATOR = 1,
		OPERATOR = 1 << 2,
		VOICE = 1 << 3
	};

	MembershipMode();
	~MembershipMode();

	const Flag	isSet(const Flag flag) const;
	void		setFlag(const Flag flag);
	void		unsetFlag(const Flag flag);

private:
	unsigned	_flags;
};

class Channel : public ChannelMode
{
public:
	Channel(const std::string &name);
	~Channel();

	void				addUser();
	const std::string	&name() const;
	const std::string	&topic() const;
	bool				isLocal() const;
	void				setKey(const std::string &topic);
	void				setTopic(const std::string &topic);

private:
	const std::string		_name;
	ChannelMode				_mode;
	std::string				_topic;
	std::string				_key;
	//size_t					_limit;
	//std::list<std::string>	_banMasks;
	//std::list<std::string>	_exceptionMasks;
	//std::list<std::string>	_invitationMasks;
	//bool					_isLocal;

	Channel(const Channel &other);

	Channel	&operator=(const Channel &other);
};
