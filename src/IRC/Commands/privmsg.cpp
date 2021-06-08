/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:12:17 by asoursou          #+#    #+#             */
/*   Updated: 2021/06/08 19:15:40 by mbrunel          ###   ########.fr       */
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
		else if (target->isMask())
		{
			size_t dot = target->find_last_of('.');
			std::string toplevel = target->substr(dot);
			if (!u.umode().isSet(UserMode::OPERATOR))
				writeNum(u, IrcError::nosuchnick(target->c_str()));
			else if (dot == std::string::npos)
				writeNum(u, IrcError::notoplevel(*target));
			else if (toplevel.find('*') != std::string::npos || toplevel.find('?') != std::string::npos)
				writeNum(u, IrcError::wildtoplevel(*target));
			else if ((*target)[0] == '$')
			{
				if (match((*target).substr(1), config.servername))
					network.msgToAll((MessageBuilder(u.prefix(), m.command()) << text).str(), &u);
				else
					writeNum(u, IrcError::badmask(*target));
			}
			else if ((*target)[0] == '#')
			{
				bool found = false;
				for (Network::UserMap::const_iterator it = network.users().begin(); it != network.users().end(); ++it)
					if (match(*(target + 1), it->second->socket()->host()))
					{
						it->second->writeLine((MessageBuilder(it->second->prefix(), m.command()) << text).str());
						found = true;
					}
				if (!found)
					writeNum(u, IrcError::badmask(*target));
			}
		}
	}
	return (0);
}
