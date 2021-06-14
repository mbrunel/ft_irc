#include "IrcNumeric.hpp"
#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::nick(User &u, const Message &m)
{
	if (!m.params().size())
		return (writeNum(u, IrcError::nonicknamegiven()));
	if (!u.requirements().isSet(UserRequirement::NICK) && u.umode().isSet(UserMode::RESTRICTED))
		return (writeNum(u, IrcError::restricted()));
	const Param &nick(m.params()[0]);
	if (!nick.isNickname())
		return (writeNum(u, IrcError::erroneusnickname(nick)));
	if (network.getByNickname(nick) || network.isFnick(nick))
		return (writeNum(u, IrcError::nicknameinuse(nick)));
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
	}
	else
	{
		std::string msg((MessageBuilder(oldprefix, "NICK") << u.nickname()).str());
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
