/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:10:36 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/03 14:36:35 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

int IrcServer::topic(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	const Param	&target(m.params()[0]);
	Channel		*c;

	if (target.isChannel() && (c = network.getByChannelname(target)))
	{
		const MemberMode *mode(c->findMember(&u));
		if (mode)
		{
			if (m.params().size() > 1)
			{
				if (c->mode().isSet(ChannelMode::TOPIC_SETTABLE_BY_CHANOP) &&
				!mode->isSet(MemberMode::OPERATOR))
					writeNum(u, IrcError::chanoprisneeded(target));
				else
				{
					c->setTopic(m.params()[1]);
					network.msgToChan(c, u.prefix() + ' ' + m.command() + ' ' + target + " :" + c->topic());
				}
			}
			else if (c->topic().size())
				u.writeLine(':' + prefix + " 332 " + target + " :" + c->topic());
			else
				u.writeLine(':' + prefix + " 331 " + target + " :No topic is set");
		}
		else
			writeNum(u, IrcError::notonchannel(target));
	}
	else
		writeNum(u, IrcError::nosuchchannel(target));
	return (0);
}
