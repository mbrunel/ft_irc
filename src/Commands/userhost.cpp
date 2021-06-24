#include "IrcServer.hpp"

int IrcServer::userhost(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().empty())
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	for (size_t i = 0; i < std::min(m.params().size(), 5UL); ++i)
	{
		User *us = network.getByNickname(m.params()[i]);
		UserMode umode = us->umode();
		std::string msg;
		msg += m.params()[i];
		msg += (umode.isSet(UserMode::OPERATOR) ? "*=" : "=");
		msg += (umode.isSet(UserMode::AWAY) ? "-" : "+");
		msg += us->username() + "@" + us->socket()->host();
		writeNum(u, IRC::Reply::userhostreply(msg));
	}
	return (0);
}
