#include "IrcServer.hpp"

int IrcServer::kick(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));

	Params ChanList= m.params()[0].split();
	Params UserList = m.params()[1].split();
	const Network::ChannelMap &channels = network.channels();
	Params::const_iterator i = ChanList.begin();
	Network::ChannelMap::const_iterator ic;
	bool check;
	while (i != ChanList.end())
	{
		check = false;
		ic = channels.begin();
		while (ic != channels.end())
		{
			Channel *c = ic->second;
			if ((*i) == c->name())
			{
				if (c->mode().isSet(Channel::UNMODERATED))
				{
					writeNum(u, IRC::Error::nochanmodes((*i)));
					++ic;
					check = true;
					continue;
				}
				MemberMode *mmode = c->findMember(&u);
				if (!mmode)
				{
					writeNum(u, IRC::Error::notonchannel(c->name()));
					++ic;
					check = true;
					continue;
				}
				if (!mmode->isSet(MemberMode::OPERATOR)
						&& !mmode->isSet(MemberMode::CREATOR))
				{
					writeNum(u, IRC::Error::chanoprisneeded(c->name()));
					++ic;
					check = true;
					continue;
				}
				Params::const_iterator iterUser = UserList.begin();
				while (iterUser != UserList.end())
				{
					User *ufind = network.getByNickname((*iterUser));
					if (ufind)
					{
						if (c->findMember(ufind))
						{
							IRC::MessageBuilder buf(u.prefix(), m.command());
							buf << ic->second->name() << ufind->nickname() << (m.params()[2].size() ? m.params()[2] : u.nickname());
							c->send(buf.str());
							c->delMember(ufind);
							check = true;
						}
						else
							writeNum(u, IRC::Error::usernotinchannel((*iterUser), c->name()));
					}
					++iterUser;
				}
			}
			++ic;
		}
		if (check == false)
			writeNum(u, IRC::Error::nosuchchannel((*i)));
		++i;
	}
	return (0);
}

/*
Command : kick
Params : <channel> * (, <channel>) <user> * (, <user>) [<comment>]
 */
