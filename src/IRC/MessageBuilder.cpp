/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:40:55 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 15:57:24 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iomanip>
#include "MessageBuilder.hpp"

MessageBuilder::MessageBuilder(const std::string &dst, const std::string &src, IrcNumeric code, const std::string &content)
{
	initPrefix(src);
	*this << std::setw(3) << std::setfill('0') << code;
	initAfterCommand(dst, content);
}

MessageBuilder::MessageBuilder(const std::string &dst, const std::string &src, const std::string &command, const std::string &content)
{
	initPrefix(src);
	*this << command;
	initAfterCommand(dst, content);
}

MessageBuilder::~MessageBuilder()
{}

void MessageBuilder::initPrefix(const std::string &src)
{
	*this << ':' << src << ' ';
}

void MessageBuilder::initAfterCommand(const std::string &dst, const std::string content)
{
	*this << ' ';
	if (dst.size())
		*this << dst;
	else
		*this << '*';
	*this << " :" << content;
}
