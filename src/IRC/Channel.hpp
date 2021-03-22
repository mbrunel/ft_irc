/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 10:06:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/22 15:07:09 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

class ChannelMode
{
public:
	enum Flag
	{
		ANONYMOUS = 1,
		INVITE_ONLY = 1 << 2,
		MODERATED = 1 << 3,
		RESTRICTED = 1 << 4,
		NO_OUTSIDE = 1 << 5,
		QUIET = 1 << 6,
		PRIVATE = 1 << 7,
		SECRET = 1 << 8
	};

	ChannelMode();
	~ChannelMode();

	const Flag	isSet(const Flag flag) const;
	void		setFlag(const Flag flag);
	void		unsetFlag(const Flag flag);

private:
	unsigned	_flags;
};

class Channel
{
public:
	enum Type { GLOBAL, LOCAL };

	Channel(const std::string &name);
	~Channel();


	Type	type() const;
	void	setTopic();

private:
	std::string	_name;
	std::string	_topic;
	std::string	_key;
	size_t		_limit;
	std::string	_banMask;

	Channel(const Channel &other);

	Channel	&operator=(const Channel &other);
};
