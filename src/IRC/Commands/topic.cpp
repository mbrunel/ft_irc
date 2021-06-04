/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:10:36 by asoursou          #+#    #+#             */
/*   Updated: 2021/06/04 13:57:02 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

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
		const ChannelMode &cm = c->mode();
		const MemberMode *mm = c->findMember(&u);
		if (m.params().size() > 1)
		{
			if (!mm)
				writeNum(u, IrcError::notonchannel(target));
			else if (!mm->isSet(MemberMode::OPERATOR) && cm.isSet(ChannelMode::TOPIC_SETTABLE_BY_CHANOP))
				writeNum(u, IrcError::chanoprisneeded(target));
			else
			{
				c->setTopic(m.params()[1]);
				network.msgToChan(c, (MessageBuilder(u.prefix(), m.command()) << target << c->topic()).str());
			}
		}
		else if (!mm && cm.isSet(ChannelMode::SECRET))
			writeNum(u, IrcError::notonchannel(target));
		else if (c->topic().size())
			writeNum(u, IrcReply::topic(target, c->topic()));
		else
			writeNum(u, IrcReply::notopic(target));
	}
	else
		writeNum(u, IrcError::nosuchchannel(target));
	return (0);
}
