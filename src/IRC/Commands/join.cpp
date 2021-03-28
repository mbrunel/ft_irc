/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:11:46 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/28 21:23:19 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include "IrcServer.hpp"

void IrcServer::join(BasicConnection *sender, const Message &msg)
{
	User *u;

	u = network.conToUsr(sender);

	if (u->state() != User::REGISTERED)
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
	Params channelsList(arg->split());
	Params keys;
	if (msg.params().size() > 1)
		keys = (++arg)->split();
	for (Params::const_iterator i(channelsList.begin()); i != channelsList.end(); ++i)
	{
		std::string target(i->asChannel());
		if (target.size())
		{
			if (u->joinedChannels() >= config.maxChannels)
			{
				u->writeLine(IrcError::toomanychannels(u->nickname(), target));
				continue ;
			}
			Channel *c = network.getChan(target);
			if (!c)
				c = network.newChan(target);
			if (c->findMember(u))
				continue ;
			c->addMember(u, MemberMode(c->count() ? 0 : MemberMode::CREATOR | MemberMode::OPERATOR));
			u->setJoinedChannels(u->joinedChannels() + 1);
			topic(sender, Message("TOPIC " + target));
			network.msgToChan(c, ':' + u->nickname() + ' ' + msg.command() + ' ' + target);
			// Add NAMES
		}
		else if (i->size())
			u->writeLine(IrcError::nosuchchannel(u->nickname(), *i));
	}
}
