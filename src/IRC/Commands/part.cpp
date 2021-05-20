/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:40:29 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/20 12:29:13 by asoursou         ###   ########.fr       */
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
			network.msgToChan(c, (MessageBuilder(u.prefix(), m.command()) << *chan).str());
			c->delMember(&u);
			if (!c->count())
			{
				network.remove(c);
				assert(network.getByChannelname(*chan) == NULL);
			}
		}
	}
	return (0);
}
