#include "IrcServer.hpp"

int IrcServer::topic(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().empty())
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	const IRC::Param &target = m.params()[0];
	Channel *c;
	if (!target.isChannel() || !(c = network.getByChannelname(target)))
		return (writeNum(u, IRC::Error::nosuchchannel(target)));
	if (c->type() == Channel::UNMODERATED)
		return (writeNum(u, IRC::Error::nochanmodes(target)));
	const MemberMode *mm = c->findMember(&u);
	if (!mm)
		return (writeNum(u, (c->mode().isSet(ChannelMode::SECRET) ? IRC::Error::nosuchchannel(c->name()) : IRC::Error::notonchannel(target))));
	if (m.params().size() < 2)
		return (writeNum(u, (c->topic().empty() ? IRC::Reply::notopic(target) : IRC::Reply::topic(target, c->topic()))));
	if (c->mode().isSet(ChannelMode::TOPIC_SETTABLE_BY_CHANOP) && !mm->isSet(MemberMode::OPERATOR))
		return (writeNum(u, IRC::Error::chanoprisneeded(target)));
	c->setTopic(m.params()[1]);
	c->send((IRC::MessageBuilder(u.prefix(), m.command()) << target << c->topic()).str());
	return (0);
}
