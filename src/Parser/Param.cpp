/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:24:30 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/29 19:38:10 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include "Param.hpp"
#include "Parser.hpp"

Param::Param(const std::string &value) :
std::string(value)
{}

Param::~Param()
{}

std::string Param::asChannel() const
{
	return (parse(Parser::asChannel));
}

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
	return (parse(Parser::asNickname));
}

std::string Param::asKey() const
{
	Context	c(*this);

	while (c.distance() < 24 && *c && !!std::strchr(" \f\t\v", *c))
		++c;
	if (c.distance() < 1 || *c)
		c.resetDistance();
	return (c.extract());
}

std::vector<Param> Param::split(char d) const
{
	std::vector<Param>	v;
	Context				c(*this);

	if (*c == d)
	{
		v.push_back(Param(""));
		++c;
	}
	while (*c)
	{
		c.resetDistance();
		while (*c && *c != d)
			++c;
		v.push_back(Param(c.extract()));
		if (*c == d)
		{
			++c;
			if (!*c)
				v.push_back(Param(""));
		}
	}
	return (v);
}

std::string Param::parse(bool (*parsing_func)(Context &, std::string &)) const
{
	std::string	s;
	Context		c(*this);

	if (!parsing_func(c, s) || *c)
		s.clear();
	return (s);
}
