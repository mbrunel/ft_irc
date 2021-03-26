/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:12:17 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 14:44:21 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::privmsg(BasicConnection *sender, const Message &msg)
{
	User *user;
	unsigned nb_p;

	if (!(user = userFromConnection(sender)))
		return ;
	if (user->state() != User::CONNECTED)
	{
		user->writeLine("You're not registered");
		return ;
	}
	if ((nb_p = msg.params().size()) < 2)
	{
		if (!nb_p)
			user->writeLine("No recipient");
		if (nb_p == 1)
			user->writeLine("No text to send");
		return ;
	}
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
			nickIter it;
			if ((it = allUsers.find(target.target())) != allUsers.end())
			{
				User *receiver = it->second;
				if (receiver->umode().isSet(UserMode::AWAY))
					user->writeLine("reply away");
				else
					receiver->writeLine(":" + user->nickname() + " PRIVMSG " + receiver->nickname() + " :" + msg.params().back() + "\n");
			}
			else
			{
				user->writeLine("No such nick");
			}
		}
		else if (target.type() == MsgTo::CHANNEL)
		{
			
		}
		else
		{
			user->writeLine("MASKS NOT SUPPORTED YET");
		}
		// incomplete / ugly error handling ofc
	}
}
