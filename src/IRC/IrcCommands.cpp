/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommands.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 20:42:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/24 00:34:38 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::loadCmd(const std::string &cmd, BasicConnection::Type type, cmdType handler)
{
	commands[cmdIdType(cmd, type)] = handler;
}

void IrcServer::loadIrcCommands()
{
	loadCmd("NICK", BasicConnection::USER, &IrcServer::userNick);
	loadCmd("USER", BasicConnection::USER, &IrcServer::userUser);
	loadCmd("PRIVMSG", BasicConnection::USER, &IrcServer::privmsg);
}

void IrcServer::userNick(BasicConnection *sender, const Message &msg)
{
	User *user;
	if (!(user = userFromConnection(sender)))
		return ;
	if (msg.params().empty())
	{
		sender->writeTo("No nick given\n");
		return ;
	}
	std::string nickname = Param(msg.params().back()).asNickname();
	if (nickname.empty())
	{
		sender->writeTo("erroneus nickname\n");
		return ;
	}
	if (allUsers.find(nickname) != allUsers.end())
	{
		sender->writeTo("nickname already in use\n");
		return ;
	}
	if (user->state() != User::NEED_NICK)
		allUsers.erase(user->nickname());

	// Incomplete / ugly error handling ofc
	user->setNickname(nickname);
	if (user->state() == User::NEED_NICK)
		user->setState(User::NEED_USER);
	allUsers[nickname] = user;
}

void IrcServer::userUser(BasicConnection *sender, const Message &msg)
{
	User *user;

	if (!(user = userFromConnection(sender)))
		return ;
	(void)msg;
	// Just so it exists
	user->setState(User::CONNECTED);
	user->writeTo("THIS IS THE MOTD\n");
}

void IrcServer::privmsg(BasicConnection *sender, const Message &msg)
{
	User *user;
	unsigned nb_p;

	if (!(user = userFromConnection(sender)))
		return ;
	if (user->state() != User::CONNECTED)
	{
		user->writeTo("You're not registered\n");
		return ;
	}
	if ((nb_p = msg.params().size()) < 2)
	{
		if (!nb_p)
			user->writeTo("No recipient\n");
		if (nb_p == 1)
			user->writeTo("No text to send\n");
		return ;
	}
	std::list<MsgTo> targets = msg.params().front().asMultipleMsgTo();
	for (std::list<MsgTo>::const_iterator it = targets.begin(); it != targets.end(); it++)
	{
		MsgTo target = *it;
		if (!target.isValid())
		{
			user->writeTo("bad msgto\n");
			continue ;
		}
		if (target.type() == MsgTo::NICKNAME)
		{
			nickIter it;
			if ((it = allUsers.find(target.target())) != allUsers.end())
			{
				User *receiver = it->second;
				if (receiver->isAway())
					user->writeTo("reply away\n");
				else
					receiver->writeTo(":" + user->nickname() + " PRIVMSG " + receiver->nickname() + " :" + msg.params().back() + "\n");
			}
			else
			{
				user->writeTo("No such nick\n");
			}
		}
		else if (target.type() == MsgTo::CHANNEL || target.type() == MsgTo::TARGETMASK)
		{
			user->writeTo("CHANNELS AND MASKS NOT SUPPORTED YET\n");
		}
		// incomplete / ugly error handling ofc
	}
}
