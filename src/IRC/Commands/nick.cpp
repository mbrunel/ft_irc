/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:14:24 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/30 02:37:27 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

bool parseNickMsg(User &sender, const Message &msg, std::string &nickname)
{
	if (msg.params().empty())
	{
		sender.writeLine("No nick given");
		return false;
	}
	nickname = Param(msg.params().back()).asNickname();
	if (nickname.empty())
	{
		sender.writeLine("erroneus nickname");
		return false;
	}
	return true;
}

bool checkNickErrors(User &sender, const Message &msg, Network &network, std::string &nickname)
{
	if (!parseNickMsg(sender, msg, nickname))
		return false;
	if (network.getByNickname(nickname))
	{
		sender.writeLine("nickname already in use");
		return false;
	}
	return true;
}

void IrcServer::nick(User &u, const Message &msg)
{
	std::string nickname;

	if (!checkNickErrors(u, msg, network, nickname))
		return ;
	if (u.requirements().isSet(UserRequirement::NICK))
	{
		network.remove(&u);
		u.setNickname(nickname);
		network.add(&u);
		u.unsetRequirement(UserRequirement::NICK);
		if (u.isRegistered())
			u.writeLine("THIS IS THE MOTD");
	}
}
