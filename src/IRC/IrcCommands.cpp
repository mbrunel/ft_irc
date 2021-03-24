/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommands.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 20:42:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/24 18:10:31 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::loadCmd(const std::string &cmd, BasicConnection::Type type, cmdType handler)
{
	commands[cmdIdType(cmd, type)] = handler;
}

void IrcServer::loadIrcCommands()
{
	loadCmd("JOIN", BasicConnection::USER, &IrcServer::join);
	loadCmd("NICK", BasicConnection::USER, &IrcServer::userNick);
	loadCmd("USER", BasicConnection::USER, &IrcServer::userUser);
	loadCmd("PRIVMSG", BasicConnection::USER, &IrcServer::privmsg);
}

void IrcServer::join(BasicConnection *sender, const Message &msg)
{
	User *u;

	if (!(u = userFromConnection(sender)))
		return ;
	if (u->state() != User::CONNECTED)
	{
		u->writeTo("You're not registered\n");
		return ;
	}
	if (!msg.params().size())
	{
		u->writeTo("461 JOIN :Not enough parameters\n");
		return ;
	}
	if (msg.params().size() == 1 && msg.params().front() == "0")
	{
		u->writeTo("Requested PART on all channels\n");
		return ;
	}
	Params::const_iterator arg(msg.params().begin());
	Params chan(arg->split());
	Params keys;
	if (msg.params().size() > 1)
		keys = (++arg)->split();
	for (Params::const_iterator i = chan.begin(), j = keys.begin();
	i != chan.end(); ++i)
	{
		std::string target = i->asChannel();
		if (target.size())
		{
			Channel &c(channels[target]);
			MemberMode *m = NULL;
			if (!c.name().size())
			{
				c.setName(target);
				m = &c.addUser(u);
				m->setFlags(MemberMode::CREATOR | MemberMode::OPERATOR);
			}
			else if (c.members().find(u) == c.members().end())
				m = &c.addUser(u);
			if (m)
			{
				u->writeTo("332 " + c.name() + " :" + c.topic() + "\n");
				c.broadcast(u, "JOIN " + target);
				// Add NAMES
			}
		}
		else if (i->size())
			u->writeTo("403 " + u->nickname() + ' ' + *i + " :No such channel\n");
		if (j != keys.end())
			++j;
	}
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
	Params targets = msg.params().front().split();
	for (Params::const_iterator it = targets.begin(); it != targets.end(); it++)
	{
		MsgTo target = it->asMsgTo();
		if (!target.isValid())
		{
			user->writeTo(*it + " :No such nick/channel\n");
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
