/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:12:17 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 18:15:24 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

static bool check(const std::string &prefix, User &u, const Message &m)
{
	if (!u.isRegistered())
		u.writeNum(prefix, IrcError::notregistered());
	if (!m.params().size())
		u.writeNum(prefix, IrcError::norecipient(m.command()));
	else if (m.params().size() < 2)
		u.writeNum(prefix, IrcError::notexttosend());
	else
		return true;
	return false;
}

void IrcServer::privmsg(User &u, const Message &m)
{
	if (!check(prefix, u, m))
		return ;
	Params		targets(m.params()[0].split());
	const Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (target->isNickname())
		{
			User *receiver = network.getByNickname(*target);
			if (!receiver)
				u.writeLine("No such nick");
			else
			{
				if (receiver->umode().isSet(UserMode::AWAY))
					u.writeLine("reply away");
				else
					receiver->writeLine(":" + u.prefix() + " PRIVMSG " + receiver->nickname() + " :" + text);
			}
		}
		else if (target->isChannel())
		{
			Channel *chan = network.getByChannelname(*target);
			if (!chan)
				u.writeLine("No such channel");
			else
				network.msgToChan(chan, ":" + u.prefix() + " PRIVMSG " + chan->name() + " :" + text, &u);
		}
		else if (target->size() > 0)
		{
			u.writeLine("MASKS NOT SUPPORTED YET");
		}
		else
			u.writeLine(*target + " :No such nick/channel");
	}
}
