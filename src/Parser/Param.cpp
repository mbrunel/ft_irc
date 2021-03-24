/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:24:30 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/24 13:48:19 by asoursou         ###   ########.fr       */
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

std::string Param::asNickname() const
{
	std::string	s;
	Context	c(*this);

	if (Parser::asNickname(c, s) && *c)
		s.clear();
	return (s);
}

std::list<Param> Param::split(char d) const
{
	t_ParamList	l;
	Context		c(*this);

	if (*c == d)
	{
		l.push_back(Param(""));
		++c;
	}
	while (*c)
	{
		c.resetDistance();
		while (*c && *c != d)
			++c;
		l.push_back(Param(c.extract()));
		std::cout << "next word: " << l.back() << std::endl;
		if (*c == d)
		{
			++c;
			if (!*c)
				l.push_back(Param(""));
		}
	}
	return (l);
}
