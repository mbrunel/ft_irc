#include "IrcServer.hpp"

std::string messageHelper(Network::ChannelMap::const_iterator c,
User &u,Channel::MemberMap::const_iterator me, const Message &m)
{
	std::string ret;

	ret = ":" + u.prefix() + " KICK " + c->second->name() + " ";
	ret += me->first->nickname();
	ret += (m.params()[2].size() != 0 ? " :" + m.params()[2] : "");
	return (ret);
}

int IrcServer::kick(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IrcError::needmoreparams(m.command())));

	std::vector<Param> ChanList= m.params()[0].split();
	std::vector<Param> UserList = m.params()[1].split();
	const Network::ChannelMap &channels = network.channels();
	std::vector<Param>::const_iterator i = ChanList.begin();
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
				MemberMode *mmode = c->findMember(&u);
				if (!mmode)
				{
					writeNum(u, IrcError::notonchannel(c->name()));
					++ic;
					check = true;
					continue;
				}
				if (!mmode->isSet(MemberMode::OPERATOR)
						&& !mmode->isSet(MemberMode::CREATOR))
				{
					writeNum(u, IrcError::chanoprisneeded(c->name()));
					++ic;
					check = true;
					continue;
				}

				std::vector<Param>::const_iterator iterUser = UserList.begin();
				while (iterUser != UserList.end())
				{
					const Channel::MemberMap &members = c->members();
					Channel::MemberMap::const_iterator im = members.begin();
					while (im != members.end())
					{
						if (im->first->nickname() == (*iterUser))
						{
							network.msgToChan(ic->second, messageHelper(ic, u, im, m));
							c->delMember(im->first);
							check = true;
							break;
						}
						++im;
					}
					if (im == members.end())
						writeNum(u, IrcError::usernotinchannel((*iterUser), c->name()));
					++iterUser;
				}
			}
			++ic;
		}
		if (check == false)
			writeNum(u, IrcError::nosuchchannel((*i)));
		++i;
	}
	return (0);
}

/*
Command : kick
Params : <channel> * (, <channel>) <user> * (, <user>) [<comment>]
 */
