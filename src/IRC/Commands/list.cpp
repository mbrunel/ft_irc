#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::list(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().empty())
	{
		const Network::ChannelMap channels = network.channels();
		for (Network::ChannelMap::const_iterator i = channels.begin(); i != channels.end(); ++i)
		{
			Channel *c = i->second;
			if (!c->mode().isSet(ChannelMode::SECRET) || c->findMember(&u))
				writeNum(u, IrcReply::list(c->name(), c->nbUserVisible(), c->topic()));
		}
	}
	else
	{
		Params args = m.params()[0].split();
		Channel *c;
		for (Params::const_iterator i = args.begin(); i != args.end(); ++i)
			if (i->isChannel() && (c = network.getByChannelname(*i)))
				writeNum(u, IrcReply::list(c->name(), c->nbUserVisible(), c->topic()));
			else
				writeNum(u, IrcError::nosuchchannel(*i));
	}
	writeNum(u, IrcReply::listend());
	return (0);
}
