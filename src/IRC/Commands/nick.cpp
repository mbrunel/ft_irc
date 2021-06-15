#include "IrcServer.hpp"

int IrcServer::nick(User &u, const IRC::Message &m)
{
	if (!m.params().size())
		return (writeNum(u, IRC::Error::nonicknamegiven()));
	if (!u.requirements().isSet(UserRequirement::NICK) && u.umode().isSet(UserMode::RESTRICTED))
		return (writeNum(u, IRC::Error::restricted()));
	const IRC::Param &nick(m.params()[0]);
	if (!nick.isNickname())
		return (writeNum(u, IRC::Error::erroneusnickname(nick)));
	if (network.getByNickname(nick) || network.isFnick(nick))
		return (writeNum(u, IRC::Error::nicknameinuse(nick)));
	network.remove(&u);
	std::string oldprefix = u.prefix();
	u.setNickname(nick);
	network.add(&u);
	if (u.requirements().isSet(UserRequirement::NICK))
	{
		u.unsetRequirement(UserRequirement::NICK);
		if (u.isRegistered())
			writeWelcome(u);
	}
	else
	{
		std::string msg((IRC::MessageBuilder(oldprefix, "NICK") << u.nickname()).str());
		u.writeLine(msg);
	}
	return (0);
}
