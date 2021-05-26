/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/26 15:57:06 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/26 16:40:14 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::invite(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	const Param &nick = m.params()[0], chan = m.params()[1];
	User *d;
	if (!nick.isNickname() || !(d = network.getByNickname(nick)))
		return (writeNum(u, IrcError::nosuchnick(nick)));
	Channel *c;
	if (!chan.isChannel() || !(c = network.getByChannelname(chan)))
		return (writeNum(u, IrcError::nosuchchannel(chan)));
	MemberMode *mm = c->findMember(&u);
	if (!mm)
		return (writeNum(u, IrcError::notonchannel(chan)));
	if (c->findMember(d))
		return (writeNum(u, IrcError::useronchannel(nick, chan)));
	if (c->mode().isSet(ChannelMode::INVITE_ONLY) && !mm->isSet(MemberMode::OPERATOR))
		return (writeNum(u, IrcError::chanoprisneeded(chan)));
	c->invite(d);
	MessageBuilder r(u.prefix(), m.command());
	r << nick << chan;
	d->writeLine(r.str());
	writeNum(u, IrcReply::inviting(chan, nick));
	if (d->umode().isSet(UserMode::AWAY))
		writeNum(u, IrcReply::away(d->nickname(), d->awayReason()));
	return (0);
}
