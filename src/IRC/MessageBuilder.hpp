/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:24:42 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 15:57:14 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sstream>
#include "IrcNumeric.hpp"

class MessageBuilder : public std::stringstream
{
public:
	MessageBuilder(const std::string &dst, const std::string &src, IrcNumeric code, const std::string &content = "");
	MessageBuilder(const std::string &dst, const std::string &src, const std::string &command, const std::string &content = "");
	virtual ~MessageBuilder();

private:
	void initPrefix(const std::string &src);
	void initAfterCommand(const std::string &dst, const std::string content);
};
