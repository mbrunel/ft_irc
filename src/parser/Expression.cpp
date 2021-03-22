/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expression.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 19:25:01 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/20 16:16:05 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Expression.hpp"

Expression::Expression() : _isValid(false)
{}

Expression::~Expression()
{}

bool Expression::isValid() const
{
	return (_isValid);
}

bool Expression::accept()
{
	return (_isValid = true);
}

bool Expression::reject()
{
	return (_isValid = false);
}

bool Expression::asCommand(Context &o, std::string &s) const
{
	o.resetDistance();
	if (std::isalpha(*o))
		while (std::isalpha(*(++o)));
	else if (!std::isdigit(*o))
		return (false);
	else
		while ((++o).distance() < 3)
			if (!std::isdigit(*o))
				return (false);
	s = o.extract();
	return (true);
}

bool Expression::asHost(Context &o, std::string &s) const
{
	Context ipv4(o), ipv6(o);

	if (isIPv4(ipv4))
		o = ipv4;
	else if (isIPv6(ipv6))
		o = ipv6;
	else
		return (asHostname(o, s));
	s = o.extract();
	return (o.distance() > 0);
}

bool Expression::asHostname(Context &o, std::string &s) const
{
	o.resetDistance();
	if (!isShortname(o))
		return (false);
	while (*o == '.')
		if (!isShortname(++o))
			return (false);
	s = o.extract();
	return (true);
}

bool Expression::asNickname(Context &o, std::string &s) const
{
	if (!std::isalpha(*o) && !o.isSpecial())
		return (false);
	o.resetDistance();
	do
		++o;
	while (*o && (std::isalnum(*o) || o.isSpecial() || *o == '-') &&
	o.distance() < 9);
	s = o.extract();
	return (true);
}

bool Expression::asUser(Context &o, std::string &s) const
{
	o.resetDistance();
	while (!o.isSpace() && *o != '@')
		++o;
	s = o.extract();
	return (o.distance() > 0);
}

bool Expression::isNumber(Context &o, size_t min, size_t max) const
{
	size_t n = 0;

	if (!std::isdigit(*o) || (*o == '0' && std::isdigit(*(++o))))
		return (false);
	while (std::isdigit(*o))
	{
		n = 10 * n + *o - '0';
		if (n > max)
			return (false);
		++o;
	}
	return (n >= min);
}

bool Expression::isXNumber(Context &o, size_t min, size_t max) const
{
	size_t n = 0;

	if (!std::isxdigit(*o) || (*o == '0' && std::isxdigit(*(++o))))
		return (false);
	while (std::isxdigit(*o))
	{
		n = 16 * n + *o - (std::isdigit(*o) ? *o - '0' : std::tolower(*o) - 'a');
		if (n > max)
			return (false);
		++o;
	}
	return (n >= min);
}

bool Expression::isIPv4(Context &o) const
{
	o.resetDistance();
	if (!isNumber(o, 0, 255))
		return (false);
	for (size_t i = 0; i < 3; ++i)
		if (*o != '.' || !isNumber(++o, 0, 255))
			return (false);
	return (true);
}

bool Expression::isIPv6(Context &o) const
{
	o.resetDistance();
	if (!isXNumber(o, 0, 0xffff))
		return (false);
	for (size_t i = 0; i < 7; ++i)
		if (*o != '.' || !isXNumber(++o, 0, 0xffff))
			return (false);
	return (true);
}

bool Expression::isShortname(Context &o) const
{
	size_t	prevDist = o.distance();
	char	last;

	if (std::isalnum(*o))
		do
			last = *o;
		while (std::isalnum(*(++o)) || *o == '-');
	return (o.distance() - prevDist > 1 && std::isalnum(last));
}
