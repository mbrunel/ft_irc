#include "IrcServer.hpp"

int IrcServer::stats(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() > 1 && m.params()[1] != config.servername)
		return (writeNum(u, IrcError::nosuchserver(m.params()[1])));
	char c = m.params().size() && m.params()[0].size() == 1 ? m.params()[0][0] : 0;
	if (c == 'm')
		for (commandsStatsMap::const_iterator i = commandsStats.begin(); i != commandsStats.end(); ++i)
			writeNum(u, IrcReply::statscommands(i->first, i->second.count, i->second.byteCount, 0));
	else if (c == 'o')
		for (Network::OperMap::const_iterator i = network.opers().begin(); i != network.opers().end(); ++i)
			writeNum(u, IrcReply::statoline(i->second.host, i->first));
	else if (c == 'u')
		writeNum(u, IrcReply::statuptime(::time(NULL) - creation));
	else
		return (0);
	writeNum(u, IrcReply::endofstats(m.params()[0]));
	return (0);
}
