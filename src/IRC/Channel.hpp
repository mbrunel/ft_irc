/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:06:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 15:07:46 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <string>
#include "Mode.hpp"
#include "User.hpp"

class Membership : public Mode
{
public:
	enum Flag
	{
		CREATOR = 1,
		OPERATOR = 1 << 2,
		VOICE = 1 << 3
	};

	Membership();
	virtual ~Membership();
};

class Channel : public Mode
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

	Channel(const std::string &name);
	virtual ~Channel();

	Membership			&addUser(User *user);
	bool				isLocal() const;
	const std::string	&name() const;
	const std::string	&topic() const;
	const std::string	&key() const;
	void				setTopic(const std::string &topic);
	void				setKey(const std::string &key);

private:
	typedef std::map<User *, Membership>	t_MembershipMap;

	const std::string	_name;
	std::string			_topic;
	std::string			_key;
	t_MembershipMap		_members;

	//size_t					_limit;
	//std::list<std::string>	_banMasks;
	//std::list<std::string>	_exceptionMasks;
	//std::list<std::string>	_invitationMasks;
	//bool					_isLocal;

	Channel(const Channel &other);

	Channel	&operator=(const Channel &other);
};
