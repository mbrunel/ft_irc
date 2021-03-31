/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:14:24 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/31 16:28:42 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include "IrcServer.hpp"

static bool check(const std::string &prefix, User &u, const Message &m, Network &network)
{
	if (!m.params().size())
		u.writeLine(IrcError::nonicknamegiven(prefix));
	else if (!m.params()[0].isNickname())
		u.writeLine(IrcError::erroneusnickname(prefix, m.params()[0]));
	else if (network.getByNickname(m.params()[0]))
		u.writeLine(IrcError::nicknameinuse(prefix, m.params()[0]));
	else
		return true;
	return false;
}

void IrcServer::nick(User &u, const Message &m)
{
	if (!check(prefix, u, m, network))
		return ;
	network.remove(&u);
	u.setNickname(m.params()[0]);
	network.add(&u);
	if (u.requirements().isSet(UserRequirement::NICK))
	{
		u.unsetRequirement(UserRequirement::NICK);
		if (u.isRegistered())
			u.writeLine("THIS IS THE MOTD");
	}
	// Add broadcast to all servers
}
