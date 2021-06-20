#include "IrcServer.hpp"

int IrcServer::nick(User &u, const IRC::Message &m)
{
	if (u.type() == User::SERVICE)
		return (writeNum(u, IRC::Error::alreadyregistred()));
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
		{
			writeWelcome(u);
			network.addNickToHistory(u);
		}
		else if (u.requirements().flags() == UserRequirement::PASS)
		{
			writeNum(u, IRC::Error::passwdmissmatch());
			disconnect(u, "Bad Password");
		}
	}
	else
	{
		std::string msg((IRC::MessageBuilder(oldprefix, m.command()) << u.nickname()).str());
		const Network::ChannelMap &channels = network.channels();
		Network::ChannelMap::const_iterator c = channels.begin();
		network.resetUserReceipt();
		while (c != channels.end())
		{
			Channel *chan = c->second;
			if (chan->findMember(&u))
			{
				chan->send(msg);
				chan->markAllMembers();
			}
			++c;
		}
		if (!u.joinedChannels())
			u.writeLine(msg);
		network.addNickToHistory(u);
	}
	return (0);
}
