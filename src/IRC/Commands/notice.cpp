/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/06 10:25:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/06/06 10:36:06 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::notice(User &u, const Message &m)
{
	if (!u.isRegistered() || m.params().size() < 2)
		return (1);
	Params		targets(m.params()[0].split());
	const Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
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
				network.msgToChan(chan, (MessageBuilder(u.prefix(), m.command()) << chan->name() << text).str(), &u);
		}
		else if (target->size() > 0)
		{
			u.writeLine("MASKS NOT SUPPORTED YET");
		}
	}
	return (0);
}
