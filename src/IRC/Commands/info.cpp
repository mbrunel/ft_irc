#include "IrcServer.hpp"
#include "ft.hpp"

int IrcServer::info(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size())
		if (!ft::match(m.params()[0].mask(), config.servername))
			return (writeNum(u, IrcError::nosuchserver(m.params()[0])));
	if (!config.info.size())
		writeNum(u, IrcReply::info(config.shortinfo));
	else
	{
		writeNum(u, IrcReply::info(config.info.front()));
		for (std::vector<std::string>::iterator it = ++config.info.begin(); it != config.info.end(); ++it)
			writeNum(u, IrcReply::info(*it));
	}
	return writeNum(u, IrcReply::endofinfo());
}
