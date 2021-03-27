/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:11:46 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/27 15:58:53 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
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
			ChannelMap::iterator res(channels.find(target));
			bool found(res != channels.end());
			if (!found)
				channels.insert(ChannelMap::value_type(target, Channel(target)));
			Channel &c(found ? res->second : channels[target]);
			if (c.findMember(u))
				continue ;
			c.addMember(u, MemberMode(c.count() ? 0 : MemberMode::CREATOR | MemberMode::OPERATOR));
			u->setJoinedChannels(u->joinedChannels() + 1);
			topic(sender, Message("TOPIC " + target));
			broadcast(c, ':' + u->nickname() + ' ' + msg.command() + ' ' + target);
			// Add NAMES
		}
		else if (i->size())
			u->writeLine(IrcError::nosuchchannel(u->nickname(), *i));
	}
}
