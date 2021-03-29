/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:10:36 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/29 18:40:43 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include "IrcServer.hpp"

void IrcServer::topic(User &u, const Message &msg)
{
	if (!u.isRegistered())
		return ;
	if (!msg.params().size())
	{
		u.writeLine(IrcError::needmoreparams(u.nickname(), msg.command()));
		return ;
	}
	Params::const_iterator args = msg.params().begin();
	std::string	target(args->asChannel());
	Channel *chan = network.getByChannelname(target);
	if (target.size() && chan)
	{
		const MemberMode *m(chan->findMember(&u));
		if (m)
		{
			if (++args != msg.params().end())
			{
				if (chan->mode().isSet(ChannelMode::TOPIC_SETTABLE_BY_CHANOP) &&
				!m->isSet(MemberMode::OPERATOR))
					u.writeLine(IrcError::chanoprisneeded(u.nickname(), msg.command()));
				else
				{
					chan->setTopic(*args);
					if (chan->topic().size())
						network.msgToChan(chan, u.nickname() + " 332 " + chan->name() + " :" + chan->topic());
				}
			}
			else if (chan->topic().size())
				u.writeLine(":aled.broken.net 332 " + chan->name() + " :" + chan->topic());
			else
				u.writeLine(":aled.broken.net 331 " + chan->name() + " :No topic is set");
		}
		else
			u.writeLine(IrcError::notonchannel(u.nickname(), msg.command()));
	}
	else
		u.writeLine(IrcError::nosuchchannel(u.nickname(), *args));
}
