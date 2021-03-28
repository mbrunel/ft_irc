/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:14:24 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/28 21:19:59 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

bool parseNickMsg(BasicConnection *sender, const Message &msg, std::string &nickname)
{
	if (msg.params().empty())
	{
		sender->writeLine("No nick given");
		return false;
	}
	nickname = Param(msg.params().back()).asNickname();
	if (nickname.empty())
	{
		sender->writeLine("erroneus nickname");
		return false;
	}
	return true;
}

bool checkNickErrors(BasicConnection *sender, const Message &msg, Network &network, std::string &nickname)
{
	if (!parseNickMsg(sender, msg, nickname))
		return false;
	if (network.isUser(nickname))
	{
		sender->writeLine("nickname already in use");
		return false;
	}
	return true;
}

void IrcServer::unknownNick(BasicConnection *sender, const Message &msg)
{
	std::string nickname;

	if (!checkNickErrors(sender, msg, network, nickname))
		return ;
	User *usr = network.newUser(sender);
	delete sender;
	network.addConnection(usr);
	usr->setNickname(nickname);
	usr->setState(User::HAS_NICK);
}

void IrcServer::userNick(BasicConnection *sender, const Message &msg)
{
	User *user;
	std::string nickname;

	if (!checkNickErrors(sender, msg, network, nickname))
		return ;
	user = network.conToUsr(sender);
	assert(user->state() != User::HAS_NOTHING);
	if (user->state() != User::HAS_NICK)
	{
		network.removeUser(user);
		user->setNickname(nickname);
		network.addUser(user);
		if (user->state() != User::REGISTERED)
		{
			user->setState(User::REGISTERED);
			user->writeLine("THIS IS THE MOTD");
		}
	}
}
