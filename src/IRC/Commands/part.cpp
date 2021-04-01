/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:40:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 18:12:24 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

static bool check(const std::string &prefix, User &u, const Message &m)
{
	if (!u.isRegistered())
		u.writeNum(prefix, IrcError::notregistered());
	else if (!m.params().size())
		u.writeNum(prefix, IrcError::needmoreparams(m.command()));
	else
		return true;
	return false;
}

void IrcServer::part(User &u, const Message &m)
{
	if (!check(prefix, u, m))
		return ;
	if (m.params()[0] == "0")
	{
		u.writeLine(':' + prefix + " :Requested PART on all channels");
		return ;
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
			network.msgToChan(c, ':' + u.prefix() + ' ' + m.command() + " :" + *chan);
			// Add NAMES
		}
		else
			writeNum(u, IrcError::toomanychannels(*chan));
	}
}
