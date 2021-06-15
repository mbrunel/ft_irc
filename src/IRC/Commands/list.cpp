#include "IrcServer.hpp"

int IrcServer::list(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().empty())
	{
		const Network::ChannelMap channels = network.channels();
		for (Network::ChannelMap::const_iterator i = channels.begin(); i != channels.end(); ++i)
		{
			Channel *c = i->second;
			if (!c->mode().isSet(ChannelMode::PRIVATE | ChannelMode::SECRET) || c->findMember(&u))
				writeNum(u, IRC::Reply::list(c->name(), c->nbUserVisible(), c->topic()));
		}
	}
	else
	{
		Params args = m.params()[0].split();
		Channel *c;
		for (Params::const_iterator i = args.begin(); i != args.end(); ++i)
			if (i->isChannel() && (c = network.getByChannelname(*i)) && (!c->mode().isSet(ChannelMode::SECRET) || c->findMember(&u)))
				writeNum(u, IRC::Reply::list(c->name(), c->nbUserVisible(), c->topic()));
			else
				writeNum(u, IRC::Error::nosuchchannel(*i));
	}
	writeNum(u, IRC::Reply::listend());
	return (0);
}
