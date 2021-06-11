#include "IrcServer.hpp"
#include "MessageBuilder.hpp"
#include "Utils.hpp"

static bool parseNextKeyParam(const std::vector<Param> &params, size_t &i, Param &dst)
{
	if (i >= params.size())
		return (0);
	dst = params[i++];
	return (1);
}

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
	Params	channels(m.params()[0].split());
	Params	keys;
	size_t	keysI = 0;
	Param	key;
	bool	needMoreParams = 0;
	
	if (m.params().size() > 1)
		keys = m.params()[1].split();
	for (Params::const_iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (!it->isChannel())
			writeNum(u, IrcError::nosuchchannel(*it));
		else if (u.joinedChannels() < config.maxChannels)
		{
			bool isBanned = 0, isInvited = 0;
			Channel *c = network.getByChannelname(*it);
			if (!c)
			{
				c = new Channel(*it);
				network.add(c);
			}
			else if (c->findMember(&u))
				continue ;
			else
			{
				const ChannelMode cm = c->mode();
				isBanned = c->isBanned(&u);
				isInvited = c->isInvited(&u);
				if (!isInvited)
				{
					if (cm.isSet(ChannelMode::INVITE_ONLY))
					{
						writeNum(u, IrcError::inviteonlychan(*it));
						continue ;
					}
					if (isBanned)
					{
						writeNum(u, IrcError::bannedfromchan(*it));
						continue ;
					}
				}
				if (cm.isSet(ChannelMode::LIMIT) && c->count() == c->limit())
				{
					writeNum(u, IrcError::channelisfull(*it));
					continue ;
				}
				if (cm.isSet(ChannelMode::KEY))
				{
					if (!parseNextKeyParam(keys, keysI, key))
					{
						writeNum(u, IrcError::needmoreparams(m.command()));
						continue ;
					}
					else if (key != c->key())
					{
						writeNum(u, IrcError::badchannelkey(*it));
						continue ;
					}
				}
			}
			c->addMember(&u, MemberMode(c->count() || c->type() == Channel::UNMODERATED ? 0 : MemberMode::CREATOR | MemberMode::OPERATOR));
			c->send((MessageBuilder(u.prefix(), m.command()) << *it).str());
			if (c->count() == 1)
			{
				if (c->type() == Channel::UNMODERATED)
				{
					c->setMode(ChannelMode(ChannelMode::TOPIC_SETTABLE_BY_CHANOP));
					mode(u, Message("MODE " + *it));
				}
			}
			else
				topic(u, Message("TOPIC " + *it));
			names(u, Message("NAMES " + *it));
		}
		else
			writeNum(u, IrcError::toomanychannels(*it));
	}
	return (needMoreParams ? writeNum(u, IrcError::needmoreparams(m.command())) : 0);
}
