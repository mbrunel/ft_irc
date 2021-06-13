#include "IrcServer.hpp"
#include "ft.hpp"

int IrcServer::whowas(User &sender, const Message &m)
{
	if (!sender.isRegistered())
		return (writeNum(sender, IrcError::notregistered()));
	if (m.params().size() == 0)
		return (writeNum(sender, IrcError::nonicknamegiven()));
	Params targets = m.params()[0].split();
	int count = -1;
	if (m.params().size() > 1)
		if (!(count = atoi(m.params()[1].c_str())))
			count = -1;
	if (m.params().size() > 2)
		if (!ft::match(m.params()[2].mask(), config.servername))
			return writeNum(sender, IrcError::nosuchserver(m.params()[2]));
	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		Network::HistoryVec h = network.getNickHistory(*target);
		if (!h.size())
			writeNum(sender, IrcError::wasnosuchnick(*target));
		else
			for (Network::HistoryVec::const_reverse_iterator it = h.rbegin(); it != h.rend() && count--; ++it)
			{
				writeNum(sender, IrcReply::whowasuser(*target, it->username, it->host, it->realname));
				writeNum(sender, IrcReply::whoisserver(*target, config.servername, config.shortinfo));
			}
	}
	return writeNum(sender, IrcReply::endofwhowas(sender.nickname()));
}
