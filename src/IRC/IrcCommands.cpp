/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommands.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 20:42:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 22:34:45 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::loadCmd(const std::string &cmd, Sender::Type type, cmdType handler)
{
	commands[cmdIdType(cmd, type)] = handler;
}

void IrcServer::loadIrcCommands()
{
	loadCmd("NICK", Sender::USER, &IrcServer::userNick);
	loadCmd("USER", Sender::USER, &IrcServer::userUser);
	loadCmd("PRIVMSG", Sender::USER, &IrcServer::privmsg);
}

User *IrcServer::userFromSender(Sender *sender)
{
	User *user;
	if (!(user = dynamic_cast<User *>(sender)))
	{
		log() << "sender is not a user" << std::endl;
		return NULL;
	}
	return user;
}

void IrcServer::userNick(Sender *sender, const Message &msg)
{
	User *user;
	if (!(user = userFromSender(sender)))
		return ;
	std::string nickname = Param(msg.params().back()).asNickname();
	if (nickname.empty())
	{
		sender->writeTo("erroneus nickname\n");
	}
	if (allUsers.find(nickname) != allUsers.end())
	{
		sender->writeTo("nickname already in use\n");
		return ;
	}
	// Incomplete error handling ofc
	user->setNickname(nickname);
	user->setState(User::NEED_USER);
	allUsers[nickname] = user;
}

void IrcServer::userUser(Sender *sender, const Message &msg)
{
	User *user;

	if (!(user = userFromSender(sender)))
		return ;
	(void)msg;
	// Just so it exists
	user->setState(User::CONNECTED);
	user->writeTo("THIS IS THE MOTD\n");
}

void IrcServer::privmsg(Sender *sender, const Message &msg)
{
	User *user;

	if (!(user = userFromSender(sender)))
		return ;
	for (std::list<Param>::const_iterator it = msg.params().begin(); it != msg.params().end(); it++)
	{
		MsgTo target = it->asMsgTo();
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
					receiver->writeTo(msg.entry());
			}
		}
		else if (target.type() == MsgTo::CHANNEL || target.type() == MsgTo::TARGETMASK)
		{
			user->writeTo("CHANNELS AND MASKS NOT SUPPORTED YET\n");
		}
		// incomplete error handling ofc
	}
}
