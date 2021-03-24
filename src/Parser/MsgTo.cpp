/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MsgTo.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:46:23 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/24 13:38:17 by asoursou         ###   ########.fr       */
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
	if (!Parser::asNickname(o, _target))
		return (reject());
	_type = NICKNAME;
	return (accept());
}

std::ostream &operator<<(std::ostream &o, const MsgTo &m)
{
	return (o << m.target());
}
