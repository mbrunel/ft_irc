#include "IrcServer.hpp"
#include "ft.hpp"

int IrcServer::whois(User &sender, const Message &m)
{
	if (!sender.isRegistered())
		return writeNum(sender, IrcError::notregistered());
	if (m.params().size() < 1)
		return writeNum(sender, IrcError::nonicknamegiven());
	Params masks;
	if (m.params().size() == 1)
		masks = Params(m.params()[0].split());
	else
	{
		if (m.params()[0] != config.servername)
			return writeNum(sender, IrcError::nosuchserver(m.params()[0]));
		masks = Params(m.params()[1].split());
	}
	for (Params::const_iterator mask = masks.begin(); mask != masks.end(); ++mask)
	{
		bool found = false;
		for (Network::UserMap::const_iterator uit = network.users().begin(); uit != network.users().end(); ++uit)
		{
			User *u = uit->second;
			if (ft::match(mask->mask(), u->nickname()))
			{
				if (!found)
					found = true;
				writeNum(sender, IrcReply::whoisuser(u->nickname(), u->username(), u->socket()->host(), u->realname()));
				writeNum(sender, IrcReply::whoisserver(u->nickname(), config.servername, config.shortinfo));
				for (Network::ChannelMap::const_iterator cit = network.channels().begin(); cit != network.channels().end(); ++cit)
				{
					Channel *c = cit->second;
					MemberMode *m = c->findMember(u);
					std::string status;
					if (!m)
						continue ;
					if (m->isSet(m->OPERATOR))
						status = "@";
					else if (m && m->isSet(m->VOICE))
						status = "+";
					writeNum(sender, IrcReply::whoischannels(u->nickname(), status, c->name()));
				}
				if (u->umode().isSet(UserMode::LOCAL_OPERATOR|UserMode::OPERATOR))
					writeNum(sender, IrcReply::whoisoperator(u->nickname()));
				writeNum(sender, IrcReply::whoisidle(u->nickname(), ::time(NULL) - u->idle()));
			}
		}
		if (!found)
			writeNum(sender, IrcError::nosuchnick(mask->mask()));
	}
	return (writeNum(sender, IrcReply::endofwhois(sender.nickname())));
}
