#include "IrcServer.hpp"
#include "libft.hpp"

int IrcServer::stats(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() > 1 && !ft::match(m.params()[1], config.servername))
		return (writeNum(u, IRC::Error::nosuchserver(m.params()[1])));
	char c = m.params().size() && m.params()[0].size() == 1 ? m.params()[0][0] : 0;
	if (c == 'm')
		for (commandsStatsMap::const_iterator i = commandsStats.begin(); i != commandsStats.end(); ++i)
			writeNum(u, IRC::Reply::statscommands(i->first, i->second.count, i->second.byteCount, 0));
	else if (c == 'o')
		for (IrcServerConfig::OperMap::const_iterator i = config.opers.begin(); i != config.opers.end(); ++i)
			writeNum(u, IRC::Reply::statoline(i->second.host, i->first));
	else if (c == 'u')
		writeNum(u, IRC::Reply::statuptime(::time(NULL) - creation));
	else
		return (0);
	writeNum(u, IRC::Reply::endofstats(m.params()[0]));
	return (0);
}
