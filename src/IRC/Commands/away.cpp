#include "IrcServer.hpp"

int IrcServer::away(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().empty())
	{
		writeNum(u, IrcReply::unaway());
		u.setUmode(UserMode(u.umode().flags() & ~UserMode::AWAY));
	}
	else
	{
		writeNum(u, IrcReply::nowaway());
		u.setUmode(UserMode(u.umode().flags() | UserMode::AWAY));
		u.setAway(m.params()[0]);
	}
	return (0);
}
