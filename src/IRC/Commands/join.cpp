#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::join(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	if (m.params()[0] == "0")
	{
		const Network::ChannelMap &channels = network.channels();
		Network::ChannelMap::const_iterator i = channels.begin();
		while (i != channels.end())
		{
			Channel *c = i->second;
			++i;
			if (c->findMember(&u))
				part(u, Message("PART " + c->name()));
		}
		return (0);
	}
	std::vector<Param>	channels(m.params()[0].split());
	std::vector<Param>	keys;
	
	if (m.params().size() > 1)
		keys = m.params()[1].split();
	for (Params::const_iterator chan(channels.begin()); chan != channels.end(); ++chan)
	{
		if (!chan->isChannel())
			writeNum(u, IrcError::nosuchchannel(*chan));
		else if (u.joinedChannels() < config.maxChannels)
		{
			Channel *c = network.getByChannelname(*chan);
			if (!c)
			{
				c = new Channel(*chan);
				network.add(c);
			}
			if (c->findMember(&u))
				continue ;
			c->addMember(&u, MemberMode(c->count() ? 0 : MemberMode::CREATOR | MemberMode::OPERATOR));
			topic(u, Message("TOPIC " + *chan));
			network.msgToChan(c, (MessageBuilder(u.prefix(), m.command()) << *chan).str());
			// names(u, Message("NAMES " + *chan));
		}
		else
			writeNum(u, IrcError::toomanychannels(*chan));
	}
	return (0);
}
