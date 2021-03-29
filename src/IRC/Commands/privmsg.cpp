/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:12:17 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/29 19:39:25 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

bool checkprivmsg(const Message &msg, User &user, std::string &text)
{
	unsigned nb_p;

	if (!user.isRegistered())
	{
		user.writeLine("You're not registered");
		return false;
	}
	if ((nb_p = msg.params().size()) < 2)
	{
		if (!nb_p)
			user.writeLine("No recipient");
		if (nb_p == 1)
			user.writeLine("No text to send");
		return false;
	}
	for (IrcServer::Params::const_iterator i = ++msg.params().begin(); i != msg.params().end(); ++i)
	{
		text += " " + *i;
	}
	return true;
}

void IrcServer::privmsg(User &u, const Message &msg)
{
	std::string text;

	if (!checkprivmsg(msg, u, text))
		return ;
	Params targets = msg.params().front().split();
	for (Params::const_iterator it = targets.begin(); it != targets.end(); it++)
	{
		MsgTo target = it->asMsgTo();
		if (!target.isValid())
		{
			u.writeLine(*it + " :No such nick/channel");
			continue ;
		}
		if (target.type() == MsgTo::NICKNAME)
		{
			User *receiver = network.getByNickname(target.target());
			if (!receiver)
				u.writeLine("No such nick");
			else
			{
				if (receiver->umode().isSet(UserMode::AWAY))
					u.writeLine("reply away");
				else
					receiver->writeLine(":" + u.nickname() + " PRIVMSG " + receiver->nickname() + " :" + text);
			}
		}
		else if (target.type() == MsgTo::CHANNEL)
		{
			Channel *chan = network.getByChannelname(target.target());
			if (!chan)
				u.writeLine("No such channel");
			else
				network.msgToChan(chan, ":" + u.nickname() + " PRIVMSG " + chan->name() + " :" + text, &u);
		}
		else
		{
			u.writeLine("MASKS NOT SUPPORTED YET");
		}
		// incomplete / ugly error handling ofc
	}
}
