#include "IrcServer.hpp"

int IrcServer::motd(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() > 0 && m.params()[0] != config.servername)
	{
		Server *s(network.getByServername(m.params()[0]));
		if (s)
			s->writeLine(m.entry());
	}
	else
		writeMotd(u);
	return (0);
}
