/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MsgTo.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:46:23 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/24 15:41:04 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MsgTo.hpp"
#include "Parser.hpp"

MsgTo::MsgTo() :
_type(UNKNOWN)
{}

MsgTo::~MsgTo()
{}

MsgTo::Type MsgTo::type() const
{
	return (_type);
}

const std::string &MsgTo::target() const
{
	return (_target);
}

bool MsgTo::interpret(Context &o)
{
	Context channelContext(o);

	if (Parser::asNickname(o, _target))
		_type = NICKNAME;
	else if (Parser::asChannel(channelContext, _target))
	{
		_type = CHANNEL;
		o = channelContext;
	}
	// else if mask
	else
		return (reject());
	return (accept());
}

std::ostream &operator<<(std::ostream &o, const MsgTo &m)
{
	return (o << m.target());
}
