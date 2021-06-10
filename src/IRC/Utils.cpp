/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 18:41:33 by asoursou          #+#    #+#             */
/*   Updated: 2021/06/10 18:58:35 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

// std::string goes brrr
static bool match_internal(const char *mask, const char *str)
{
	const char m = *mask;
	const char c = *str;
	if (!m)
		return (!c);
	if (m == '\\') // escape character (only if next is a special character)
	{
		const char m1 = mask[1];
		if (m1 == '*' || m1 == '?')
			return (m1 == c ? match_internal(mask + 2, str + 1) : 0);
	}
	if (m == '*') // wildcard matching
	{
		if (match_internal(mask + 1, str))
			return (1);
		if (!c)
			return (0);
	}
	else
	{
		if (m == '?') // any character matching
		{
			if (!c)
				return (0);
		}
		else if (m != c) // else character must match
			return (0);
		++mask;
	}
	return (match_internal(mask, str + 1));
}

bool Utils::match(const std::string &mask, const std::string &str)
{
	return (match_internal(mask.c_str(), str.c_str()));
}

std::string Utils::to_date(const time_t t, const char *format)
{
	char buf[80];

	strftime(buf, sizeof(buf), format, localtime(&t));
	return (buf);
}
