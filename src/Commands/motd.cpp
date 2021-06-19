#include "IrcServer.hpp"

int IrcServer::motd(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() > 0 && m.params()[0] != config.servername)
		writeNum(u, IRC::Error::nosuchserver(m.params()[0]));
	else
		writeMotd(u);
	return (0);
}
