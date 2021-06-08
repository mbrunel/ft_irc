/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:40:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/06/08 15:53:38 by mapapin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::part(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	std::vector<Param> channels(m.params()[0].split());

	for (Params::const_iterator chan(channels.begin()); chan != channels.end(); ++chan)
	{
		Channel *c;
		if (!chan->isChannel() || !(c = network.getByChannelname(*chan)))
			writeNum(u, IrcError::nosuchchannel(*chan));
		else if (!c->findMember(&u))
			writeNum(u, IrcError::notonchannel(*chan));
		else
		{
			MessageBuilder msg(u.prefix(), m.command());
			msg << *chan;
			if (m.params().size() > 1)
				msg << m.params()[1];
			network.msgToChan(c, msg.str());
			c->delMember(&u);
			if (!c->count())
				network.remove(c);
		}
	}
	return (0);
}
