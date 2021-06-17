#include "IrcServer.hpp"
#include "libft.hpp"

#define IRC_WILDCARD "*"

int IrcServer::servlist(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	std::string mask;
	if (m.params().size())
		mask = m.params()[0].mask();
	const Network::UserMap &map = network.users();
	for (Network::UserMap::const_iterator i = map.begin(); i != map.end(); ++i)
	{
		const User *s = i->second;
		if (s->type() != User::SERVICE || (mask.size() && !ft::match(mask, s->nickname())))
			continue ;
		writeNum(u, IRC::Reply::servlist(s->nickname(), config.servername, IRC_WILDCARD, IRC_WILDCARD, s->hopcount(), s->realname()));
	}
	writeNum(u, IRC::Reply::servlistend((mask.empty() ? IRC_WILDCARD : mask), IRC_WILDCARD));
	return (0);
}
