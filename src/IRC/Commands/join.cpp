/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:11:46 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/03 15:08:57 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::join(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	if (m.params()[0] == "0")
	{
		u.writeLine(':' + prefix + " :Requested PART on all channels");
		return (0);
	}
	std::vector<Param>	channels(m.params()[0].split());
	std::vector<Param>	keys;
	
	if (m.params().size() > 1)
		keys = m.params()[1].split();
	for (Params::const_iterator chan(channels.begin()); chan != channels.end(); ++chan)
	{
		if (!chan->isChannel())
			writeNum(u, IrcError::nosuchchannel(*chan));
		else if (u.joinedChannels() < config.maxChannels)
		{
			Channel *c = network.getByChannelname(*chan);
			if (!c)
			{
				c = new Channel(*chan);
				network.add(c);
			}
			if (c->findMember(&u))
				continue ;
			c->addMember(&u, MemberMode(c->count() ? 0 : MemberMode::CREATOR | MemberMode::OPERATOR));
			topic(u, Message("TOPIC " + *chan));
			network.msgToChan(c, (MessageBuilder(u.prefix(), m.command()) << *chan).str());
			// Add NAMES
		}
		else
			writeNum(u, IrcError::toomanychannels(*chan));
	}
	return (0);
}
