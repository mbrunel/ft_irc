#include "IrcServer.hpp"

int IrcServer::whowas(User &sender, const IRC::Message &m)
{
	if (!sender.isRegistered())
		return (writeNum(sender, IRC::Error::notregistered()));
	if (m.params().size() == 0)
		return (writeNum(sender, IRC::Error::nonicknamegiven()));
	Params targets = m.params()[0].split();
	int count = 0;
	if (m.params().size() > 1)
		count = atoi(m.params()[1].c_str());
	if (m.params().size() > 2)
		if (!Utils::match(m.params()[2].mask(), config.servername))
			return writeNum(sender, IRC::Error::nosuchserver(m.params()[2]));
	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		Network::InfoVec h = network.getNickHistory(*target, count);
		if (!h.size())
			writeNum(sender, IRC::Error::wasnosuchnick(*target));
		else
			for (Network::InfoVec::iterator it = h.begin(); it != h.end(); ++it)
			{
				writeNum(sender, IRC::Reply::whowasuser(*target, it->username, it->host, it->realname));
				writeNum(sender, IRC::Reply::whoisserver(*target, config.servername, config.shortinfo));
			}
	}
	return writeNum(sender, IRC::Reply::endofwhowas(sender.nickname()));
}