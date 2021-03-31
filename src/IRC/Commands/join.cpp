/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:11:46 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/31 15:58:40 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include "IrcServer.hpp"

static bool check(const std::string &prefix, User &u, const Message &m)
{
	if (!u.isRegistered())
		u.writeLine(IrcError::notregistered(prefix));
	else if (!m.params().size())
		u.writeLine(IrcError::needmoreparams(prefix, m.command()));
	else
		return true;
	return false;
}

void IrcServer::join(User &u, const Message &m)
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
			u.writeLine(IrcError::nosuchchannel(u.prefix(), *chan));
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
			u.writeLine(IrcError::toomanychannels(u.prefix(), *chan));
	}
}
