#include "IrcServer.hpp"

int IrcServer::motd(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() > 0 && m.params()[0] != config.servername)
		writeNum(u, IrcError::nosuchserver(m.params()[0]));
	else
		writeMotd(u);
	return (0);
}
