/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:24:30 by asoursou          #+#    #+#             */
/*   Updated: 2021/06/02 13:36:03 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include "Param.hpp"
#include "Parser.hpp"

Param::Param()
{}

Param::Param(const std::string &value) :
std::string(value)
{}

Param::~Param()
{}

bool Param::isChannel() const
{
	return (check(Parser::asChannel));
}

bool Param::isMask() const
{
	Param::const_iterator	i;
	bool					prevIsEsc;

	for (i = begin(), prevIsEsc = false; i != end(); ++i)
		if ((*i == '*' || *i == '?'))
		{
			if (!prevIsEsc)
				return (true);
		}
		else
			prevIsEsc = *i == '\\';
	return (false);
}

bool Param::isNickname() const
{
	return (check(Parser::asNickname));
}

bool Param::isKey() const
{
	Context	c(*this);

	while (c.distance() < 24 && *c && !std::strchr(" \f\t\v", *c))
		++c;
	return (c.distance() > 0 && !*c);
}

std::string Param::mask() const
{
	std::string		dst;
	bool			prevIsEsc = 0;
	const_iterator	i = begin();

	dst.reserve(size() + 1);
	while (i != end())
	{
		char c = *i;
		dst.push_back(c);
		if (!prevIsEsc && c == '*')
			while (i + 1 != end() && i[1] == '*') // * == two or more wildcards
				++i;
		prevIsEsc = c == '\\';
		++i;
	}
	return (dst);
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

bool Param::check(bool (*parsing_func)(Context &, std::string &)) const
{
	std::string	s;
	Context		c(*this);

	return (parsing_func(c, s) && !*c);
}
