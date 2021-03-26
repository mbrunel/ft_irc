/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:11:46 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 18:29:57 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::join(BasicConnection *sender, const Message &msg)
{
	User *u;

	if (!(u = userFromConnection(sender)))
		return ;
	if (u->state() != User::CONNECTED)
	{
		u->writeLine("You're not registered");
		return ;
	}
	if (!msg.params().size())
	{
		u->writeLine("461 JOIN :Not enough parameters");
		return ;
	}
	if (msg.params().size() == 1 && msg.params().front() == "0")
	{
		u->writeLine("Requested PART on all channels");
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
				m->set(MemberMode::CREATOR | MemberMode::OPERATOR);
			}
			else if (c.members().find(u) == c.members().end())
				m = &c.addUser(u);
			if (m)
			{
				u->writeLine("332 " + c.name() + " :" + c.topic());
				//c.broadcast(u, "JOIN " + target);
				// Add NAMES
			}
		}
		else if (i->size())
			u->writeLine("403 " + u->nickname() + ' ' + *i + " :No such channel");
		if (j != keys.end())
			++j;
	}
}
