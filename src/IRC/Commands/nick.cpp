/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:14:24 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/03 14:34:53 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::nick(User &u, const Message &m)
{
	if (!m.params().size())
		return (writeNum(u, IrcError::nonicknamegiven()));
	if (!m.params()[0].isNickname())
		return (writeNum(u, IrcError::erroneusnickname(m.params()[0])));
	if (network.getByNickname(m.params()[0]))
		return (writeNum(u, IrcError::nicknameinuse(m.params()[0])));
	network.remove(&u);
	std::string oldprefix = u.prefix();
	u.setNickname(m.params()[0]);
	network.add(&u);
	if (u.requirements().isSet(UserRequirement::NICK))
	{
		u.unsetRequirement(UserRequirement::NICK);
		if (u.isRegistered())
		{
			writeWelcome(u);
			network.msgToNetwork((MessageBuilder(prefix, "NICK") << u.nickname() <<
			u.hopcount() << u.username() << prefix << "34" << u.umode().flags() << u.realname()).str(), &u);
		}
	}
	else
	{
		std::string msg((MessageBuilder(oldprefix, "NICK") << u.nickname()).str());
		u.writeLine(msg);
		network.msgToNetwork(msg, &u);
	}
	return (0);
}
