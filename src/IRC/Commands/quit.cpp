#include "IrcServer.hpp"

int IrcServer::quit(User &u, const Message &m)
{
	if (!u.isRegistered() || m.params().empty())
		disconnect(u, "");
	else
		disconnect(u, m.params()[0]);
	return (0);
}
