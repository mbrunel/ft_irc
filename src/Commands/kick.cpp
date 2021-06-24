#include "IrcServer.hpp"

int IrcServer::kick(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	Params chans = m.params()[0].split(), users = m.params()[1].split();
	if (chans.size() > 1 && chans.size() != users.size())
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	Params::const_iterator i = users.begin();
	for (Params::const_iterator j = chans.begin(); j != chans.end(); ++j)
	{
		Channel *c = network.getByChannelname(*j);
		if (!c)
		{
			writeNum(u, IRC::Error::nosuchchannel(*j));
			continue ;
		}
		if (c->mode().isSet(Channel::UNMODERATED))
		{
			writeNum(u, IRC::Error::nochanmodes(*j));
			continue;
		}
		MemberMode *mmode = c->findMember(&u);
		if (!mmode)
		{
			writeNum(u, IRC::Error::notonchannel(c->name()));
			continue;
		}
		if (!mmode->isSet(MemberMode::OPERATOR) && !mmode->isSet(MemberMode::CREATOR))
		{
			writeNum(u, IRC::Error::chanoprisneeded(c->name()));
			continue;
		}
		for (; i != users.end(); ++i)
		{
			User *ufind = network.getByNickname(*i);
			if (ufind && c->findMember(ufind))
			{
				IRC::MessageBuilder buf(u.prefix(), m.command());
				buf << c->name() << ufind->nickname();
				if (m.params().size() > 2)
					buf << m.params()[2];
				else
					buf << "no reason";
				c->send(buf.str());
				c->delMember(ufind);
			}
			else
				writeNum(u, IRC::Error::usernotinchannel(*i, c->name()));
			if (chans.size() != 1)
				break ;
		}
		if (!c->count())
			network.remove(c);
	}
	return (0);
}

/*
Command : kick
Params : <channel> * (, <channel>) <user> * (, <user>) [<comment>]
 */
