/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:12:17 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/28 22:13:39 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

bool checkprivmsg(const Message &msg, User *user, std::string &text)
{
	unsigned nb_p;

	if (user->state() != User::REGISTERED)
	{
		user->writeLine("You're not registered");
		return false;
	}
	if ((nb_p = msg.params().size()) < 2)
	{
		if (!nb_p)
			user->writeLine("No recipient");
		if (nb_p == 1)
			user->writeLine("No text to send");
		return false;
	}
	for (std::list<Param>::const_iterator it = ++msg.params().begin(); it != msg.params().end(); it++)
	{
		text += " " + *it;
	}
	return true;
}

void IrcServer::privmsg(BasicConnection *sender, const Message &msg)
{
	User *user = network.conToUsr(sender);
	std::string text;

	if (!checkprivmsg(msg, user, text))
		return ;
	Params targets = msg.params().front().split();
	for (Params::const_iterator it = targets.begin(); it != targets.end(); it++)
	{
		MsgTo target = it->asMsgTo();
		if (!target.isValid())
		{
			user->writeLine(*it + " :No such nick/channel");
			continue ;
		}
		if (target.type() == MsgTo::NICKNAME)
		{
			User *receiver = network.getUser(target.target());
			if (!receiver)
				user->writeLine("No such nick");
			else
			{
				if (receiver->umode().isSet(UserMode::AWAY))
					user->writeLine("reply away");
				else
					receiver->writeLine(":" + user->nickname() + " PRIVMSG " + receiver->nickname() + " :" + text);
			}
		}
		else if (target.type() == MsgTo::CHANNEL)
		{
			Channel *chan = network.getChan(target.target());
			if (!chan)
				user->writeLine("No such channel");
			else
				network.msgToChan(chan, ":" + user->nickname() + " PRIVMSG " + chan->name() + " :" + text, user);
		}
		else
		{
			user->writeLine("MASKS NOT SUPPORTED YET");
		}
		// incomplete / ugly error handling ofc
	}
}
