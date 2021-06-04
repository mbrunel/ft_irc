/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 06:25:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/06/04 15:36:43 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sstream>
#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

static inline std::string findReason(const Message &m)
{
	if (m.params().empty())
		return ("Client Quit");
	return (m.params()[0]);
}

int IrcServer::quit(User &u, const Message &m)
{
	const std::string reason = findReason(m);
	const std::string quitMessage = (MessageBuilder(u.prefix(), m.command()) << reason).str();
	std::stringstream errorReason;

	if (u.joinedChannels())
	{
		const Network::ChannelMap &channels = network.channels();
		Network::ChannelMap::const_iterator i = channels.begin();
		while (i != channels.end())
		{
			Channel *c = i->second;
			++i;
			if (c->findMember(&u))
			{
				c->delMember(&u);
				c->count() ? network.msgToChan(c, quitMessage) : network.remove(c);
			}
		}
	}
	errorReason << "Closing Link: " << u.socket()->host() << " (" << reason << ')';
	u.writeLine(quitMessage);
	writeError(u.socket(), errorReason.str());
	disconnect(u.socket());
	return (0);
}
