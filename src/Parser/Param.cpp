/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:24:30 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 18:23:37 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Param.hpp"
#include "Parser.hpp"

Param::Param(const std::string &value) :
std::string(value)
{}

Param::~Param()
{}

MsgTo Param::asMsgTo() const
{
	MsgTo	m;
	Context	c(*this);

	if (!m.interpret(c) || *c)
		return (MsgTo());
	return (m);
}

std::list<MsgTo> Param::asMultipleMsgTo() const
{
	std::list<MsgTo>	l;
	Context				c(*this);

	while (*c)
	{
		l.push_back(MsgTo());
		if (!l.back().interpret(c) || (*c && *c != ','))
		{
			l.clear();
			break ;
		}
		++c;
	}
	return (l);
}

std::string Param::asNickname() const
{
	std::string	s;
	Context	c(*this);

	Parser::asNickname(c, s);
	return (s);
}
