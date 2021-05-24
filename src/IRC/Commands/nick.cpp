/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:14:24 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/20 19:28:12 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::nick(User &u, const Message &m)
{
	if (!m.params().size())
		return (writeNum(u, IrcError::nonicknamegiven()));
	const Param &nick(m.params()[0]);
	if (!nick.isNickname())
		return (writeNum(u, IrcError::erroneusnickname(nick)));
	if (network.getByNickname(nick))
		return (writeNum(u, IrcError::nicknameinuse(nick)));
	network.remove(&u);
	std::string oldprefix = u.prefix();
	u.setNickname(nick);
	network.add(&u);
	if (u.requirements().isSet(UserRequirement::NICK))
	{
		u.unsetRequirement(UserRequirement::NICK);
		if (u.isRegistered())
		{
			writeWelcome(u);
			network.msgToNetwork((MessageBuilder(config.servername, "NICK") << u.nickname() <<
			u.hopcount() << u.username() << config.servername << "34" << u.umode().flags() << u.realname()).str(), &u);
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
