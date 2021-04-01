/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:40:55 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 18:57:22 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iomanip>
#include <sstream>
#include "MessageBuilder.hpp"

std::string MessageBuilder::create(const std::string &src, const std::string &dst, IrcNumeric response)
{
	std::stringstream s;
	s << ':' << src << ' ' << std::setw(3) << std::setfill('0') << response.code << ' ';
	if (dst.size())
		s << dst;
	else
		s << '*';
	s << ' ' << response.content;
	return (s.str());
}

std::string MessageBuilder::create(const std::string &src, const std::string &dst, const std::string &command, const std::string &content)
{
	std::stringstream s;
	s << ':' << src << ' ' << command << ' ';
	if (dst.size())
		s << dst;
	else
		s << '*';
	if (content.size())
		s << " :" << content;
	return (s.str());
}
