/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/06 10:25:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/06/10 15:49:04 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"
#include "Utils.hpp"

int IrcServer::notice(User &u, const Message &m)
{
	if (!u.isRegistered() || m.params().size() < 2)
		return (1);

	Params		targets(m.params()[0].split());
	const Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (target->isMask())
		{
			if (!u.umode().isSet(UserMode::OPERATOR))
				continue ;
			else if ((*target)[0] == '$')
			{
				if (Utils::match((*target).substr(1), config.servername))
					network.msgToAll((MessageBuilder(u.prefix(), m.command()) << text).str(), &u);
			}
			else if ((*target)[0] == '#')
			{
				size_t dot;
				std::string toplevel;

				if ((dot = target->find_last_of('.')) == std::string::npos)
					continue ;
				toplevel = target->substr(dot);
				if (toplevel.find('*') == std::string::npos && toplevel.find('?') == std::string::npos)
					for (Network::UserMap::const_iterator it = network.users().begin(); it != network.users().end(); ++it)
						if (Utils::match((*target).substr(1), it->second->socket()->host()))
							it->second->writeLine((MessageBuilder(it->second->prefix(), m.command()) << text).str());
			}
		}
		if (target->isNickname())
		{
			User *receiver = network.getByNickname(*target);
			if (receiver)
				receiver->writeLine((MessageBuilder(u.prefix(), m.command()) << text).str());
		}
		else if (target->isChannel())
		{
			Channel *chan = network.getByChannelname(*target);
			if (chan)
				chan->send((MessageBuilder(u.prefix(), m.command()) << chan->name() << text).str(), &u);
		}
	}
	return (0);
}
