/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:12:17 by asoursou          #+#    #+#             */
/*   Updated: 2021/06/08 12:19:18 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::privmsg(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IrcError::norecipient(m.command())));
	if (m.params().size() < 2)
		return (writeNum(u, IrcError::notexttosend()));
	Params		targets(m.params()[0].split());
	const Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (target->isNickname())
		{
			User *receiver = network.getByNickname(*target);
			if (!receiver)
			{
				writeNum(u, IrcError::nosuchnick(*target));
				continue ;
			}
			if (receiver->umode().isSet(UserMode::AWAY))
				writeNum(u, IrcReply::away(receiver->nickname(), receiver->awayReason()));
			receiver->writeLine((MessageBuilder(u.prefix(), m.command()) << text).str());
		}
		else if (target->isChannel())
		{
			Channel *chan = network.getByChannelname(*target);
			if (!chan)
				writeNum(u, IrcError::nosuchchannel(*target));
			else
				network.msgToChan(chan, (MessageBuilder(u.prefix(), m.command()) << chan->name() << text).str(), &u);
		}
		else if (target->size() > 0)
		{
			if (!u.umode().isSet(UserMode::OPERATOR) || (*target)[0] != '#')
			{
				writeNum(u, IrcError::nosuchnick(target->c_str());
				continue ;
			}
			for 
		}
	}
	return (0);
}
