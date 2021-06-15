#include "IrcServer.hpp"

int IrcServer::away(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().empty())
	{
		writeNum(u, IRC::Reply::unaway());
		u.setUmode(UserMode(u.umode().flags() & ~UserMode::AWAY));
	}
	else
	{
		writeNum(u, IRC::Reply::nowaway());
		u.setUmode(UserMode(u.umode().flags() | UserMode::AWAY));
		u.setAway(m.params()[0]);
	}
	return (0);
}
