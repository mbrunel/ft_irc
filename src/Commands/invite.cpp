#include "IrcServer.hpp"

int IrcServer::invite(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	const IRC::Param &nick = m.params()[0], chan = m.params()[1];
	User *d;
	if (!nick.isNickname() || !(d = network.getByNickname(nick)))
		return (writeNum(u, IRC::Error::nosuchnick(nick)));
	Channel *c;
	if (!chan.isChannel() || !(c = network.getByChannelname(chan)))
		return (writeNum(u, IRC::Error::nosuchchannel(chan)));
	MemberMode *mm = c->findMember(&u);
	if (!mm)
		return (writeNum(u, IRC::Error::notonchannel(chan)));
	if (c->findMember(d))
		return (writeNum(u, IRC::Error::useronchannel(nick, chan)));
	if (c->mode().isSet(ChannelMode::INVITE_ONLY) && !mm->isSet(MemberMode::OPERATOR))
		return (writeNum(u, IRC::Error::chanoprisneeded(chan)));
	c->invite(d);
	IRC::MessageBuilder r(u.prefix(), m.command());
	r << nick << chan;
	d->writeLine(r.str());
	writeNum(u, IRC::Reply::inviting(chan, nick));
	if (d->umode().isSet(UserMode::AWAY))
		writeNum(u, IRC::Reply::away(d->nickname(), d->awayReason()));
	return (0);
}
