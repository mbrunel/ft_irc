#include "IrcServer.hpp"
#include "libft.hpp"

int IrcServer::whois(User &sender, const IRC::Message &m)
{
	if (!sender.isRegistered())
		return writeNum(sender, IRC::Error::notregistered());
	if (m.params().size() < 1)
		return writeNum(sender, IRC::Error::nonicknamegiven());
	Params masks;
	if (m.params().size() == 1)
		masks = Params(m.params()[0].split());
	else
	{
		if (!ft::match(m.params()[0].mask(), config.servername))
			return writeNum(sender, IRC::Error::nosuchserver(m.params()[0]));
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
				writeNum(sender, IRC::Reply::whoisuser(u->nickname(), u->username(), u->socket()->host(), u->realname()));
				writeNum(sender, IRC::Reply::whoisserver(u->nickname(), config.servername, config.shortinfo));
				for (Network::ChannelMap::const_iterator cit = network.channels().begin(); cit != network.channels().end(); ++cit)
				{
					Channel *c = cit->second;
					MemberMode *m = c->findMember(u);
					std::string status;
					if (!m)
						continue ;
					if (m->isSet(m->OPERATOR))
						status = "@";
					else if (m->isSet(m->VOICE))
						status = "+";
					writeNum(sender, IRC::Reply::whoischannels(u->nickname(), status, c->name()));
				}
				if (u->umode().isSet(UserMode::OPERATOR))
					writeNum(sender, IRC::Reply::whoisoperator(u->nickname()));
				writeNum(sender, IRC::Reply::whoisidle(u->nickname(), ::time(NULL) - u->idle()));
			}
		}
		if (!found)
			writeNum(sender, IRC::Error::nosuchnick(mask->mask()));
	}
	return (writeNum(sender, IRC::Reply::endofwhois(sender.nickname())));
}
