/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:10:36 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/27 16:06:29 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include "IrcServer.hpp"

void IrcServer::topic(BasicConnection *sender, const Message &msg)
{
	User *u;

	if (!(u = userFromConnection(sender)) || u->state() != User::CONNECTED)
		return ;
	if (!msg.params().size())
	{
		u->writeLine(IrcError::needmoreparams(u->nickname(), msg.command()));
		return ;
	}
	Params::const_iterator args = msg.params().begin();
	std::string	target(args->asChannel());
	ChannelMap::iterator i(channels.find(target));
	if (target.size() && i != channels.end())
	{
		Channel &c(i->second);
		const MemberMode *m(c.findMember(u));
		if (m)
		{
			if (++args != msg.params().end())
			{
				if (c.mode().isSet(ChannelMode::TOPIC_SETTABLE_BY_CHANOP) &&
				!m->isSet(MemberMode::OPERATOR))
					u->writeLine(IrcError::chanoprisneeded(u->nickname(), msg.command()));
				else
				{
					c.setTopic(*args);
					if (c.topic().size())
						broadcast(c, ":aled.broken.net 332 " + u->nickname() + ' ' + target + " :" + c.topic());
					else
						broadcast(c, ":aled.broken.net 331 " + u->nickname() + ' ' + target + " :No topic is set");
				}
			}
			else if (c.topic().size())
				u->writeLine(":aled.broken.net 332 " + u->nickname() + ' ' + target + " :" + c.topic());
			else
				u->writeLine(":aled.broken.net 331 " + u->nickname() + ' ' + target + " :No topic is set");
		}
		else
			u->writeLine(IrcError::notonchannel(u->nickname(), msg.command()));
	}
	else
		u->writeLine(IrcError::nosuchchannel(u->nickname(), *args));
}
