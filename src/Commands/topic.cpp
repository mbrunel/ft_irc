#include "IrcServer.hpp"

int IrcServer::topic(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	const IRC::Param	&target(m.params()[0]);
	Channel				*c;

	if (target.isChannel() && (c = network.getByChannelname(target)))
	{
		if (c->type() == Channel::UNMODERATED)
			return (writeNum(u, IRC::Error::nochanmodes(target)));
		const ChannelMode &cm = c->mode();
		const MemberMode *mm = c->findMember(&u);
		if (m.params().size() > 1)
		{
			if (!mm)
			{
				if (c->mode().isSet(ChannelMode::SECRET))
					IRC::Error::nosuchchannel(c->name());
				else
					writeNum(u, IRC::Error::notonchannel(target));
			}
			else if (!mm->isSet(MemberMode::OPERATOR) && cm.isSet(ChannelMode::TOPIC_SETTABLE_BY_CHANOP))
				writeNum(u, IRC::Error::chanoprisneeded(target));
			else
			{
				c->setTopic(m.params()[1]);
				c->send((IRC::MessageBuilder(u.prefix(), m.command()) << target << c->topic()).str());
			}
		}
		else if (!mm)
		{
			if (cm.isSet(ChannelMode::SECRET))
				writeNum(u, IRC::Error::nosuchchannel(target));
			else
				writeNum(u, IRC::Error::notonchannel(target));
		}
		else if (c->topic().size())
			writeNum(u, IRC::Reply::topic(target, c->topic()));
		else
			writeNum(u, IRC::Reply::notopic(target));
	}
	else
		writeNum(u, IRC::Error::nosuchchannel(target));
	return (0);
}
