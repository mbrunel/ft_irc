/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:10:36 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 16:32:56 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

static bool check(const std::string &prefix, User &u, const Message &m)
{
	if (!u.isRegistered())
		u.writeLine(IrcError::notregistered(prefix));
	else if (!m.params().size())
		u.writeLine(IrcError::needmoreparams(u.nickname(), m.command()));
	else
		return true;
	return false;
}

void IrcServer::topic(User &u, const Message &m)
{
	if (!check(prefix, u, m))
		return ;
	const Param	&target(m.params()[0]);
	Channel		*chan;

	if (target.isChannel() && (chan = network.getByChannelname(target)))
	{
		const MemberMode *mode(chan->findMember(&u));
		if (mode)
		{
			if (m.params().size() > 1)
			{
				if (chan->mode().isSet(ChannelMode::TOPIC_SETTABLE_BY_CHANOP) &&
				!mode->isSet(MemberMode::OPERATOR))
					u.writeLine(IrcError::chanoprisneeded(u.nickname(), m.command()));
				else
				{
					chan->setTopic(m.params()[1]);
					network.msgToChan(chan, u.prefix() + ' ' + m.command() + ' ' + chan->name() + " :" + chan->topic());
				}
			}
			else if (chan->topic().size())
				u.writeLine(':' + prefix + " 332 " + chan->name() + " :" + chan->topic());
			else
				u.writeLine(':' + prefix + " 331 " + chan->name() + " :No topic is set");
		}
		else
			u.writeLine(IrcError::notonchannel(prefix, m.command()));
	}
	else
		u.writeLine(IrcError::nosuchchannel(prefix, target));
}
