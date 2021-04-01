/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:14:24 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 20:08:37 by mbrunel          ###   ########.fr       */
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
		u.setNickname(m.params()[0]);
		network.add(&u);
		if (u.requirements().isSet(UserRequirement::NICK))
		{
			u.unsetRequirement(UserRequirement::NICK);
			if (u.isRegistered())
				writeWelcome(u);
			else
				return ;
		}
		else
			u.writeLine(":" + u.prefix() + " NICK " + m.params()[0]);
	}
	// Add broadcast to all servers
}
