#include "IrcServer.hpp"

int IrcServer::part(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	Params channels(m.params()[0].split());

	for (Params::const_iterator chan(channels.begin()); chan != channels.end(); ++chan)
	{
		Channel *c;
		if (!chan->isChannel() || !(c = network.getByChannelname(*chan)))
			writeNum(u, IRC::Error::nosuchchannel(*chan));
		else if (!c->findMember(&u))
			writeNum(u, IRC::Error::notonchannel(*chan));
		else
		{
			IRC::MessageBuilder msg(u.prefix(), m.command());
			msg << *chan;
			if (m.params().size() > 1)
				msg << m.params()[1];
			c->send(msg.str());
			c->delMember(&u);
			if (!c->count())
				network.remove(c);
		}
	}
	return (0);
}
