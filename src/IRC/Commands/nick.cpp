/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:14:24 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/02 05:09:12 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

void IrcServer::nick(User &u, const Message &m)
{
	if (!m.params().size())
		writeNum(u, IrcError::nonicknamegiven());
	else if (!m.params()[0].isNickname())
		writeNum(u, IrcError::erroneusnickname(m.params()[0]));
	else if (network.getByNickname(m.params()[0]))
		writeNum(u, IrcError::nicknameinuse(m.params()[0]));
	else
	{
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
				network.msgToNetwork(":" + prefix + " NICK " + u.nickname() + " ... " + /* hopcount et tout le reste */ u.realname(), &u);
			}
		}
		else
		{
			std::string msg = ":" + oldprefix + " NICK " + m.params()[0];
			u.writeLine(msg);
			network.msgToNetwork(msg, &u);
		}
	}
}
