#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::privmsg(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IrcError::norecipient(m.command())));
	if (m.params().size() < 2)
		return (writeNum(u, IrcError::notexttosend()));
	Params		targets(m.params()[0].split());
	const Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (target->isNickname())
		{
			User *receiver = network.getByNickname(*target);
			if (!receiver)
				writeNum(u, IrcError::nosuchnick(*target));
			else
			{
				if (receiver->umode().isSet(UserMode::AWAY))
					writeNum(u, IrcReply::away(receiver->nickname(), receiver->awayReason()));
				receiver->writeLine((MessageBuilder(u.prefix(), m.command()) << text).str());
			}
		}
		else if (target->isChannel())
		{
			Channel *chan = network.getByChannelname(*target);
			if (!chan)
				writeNum(u, IrcError::nosuchchannel(*target));
			else
				network.msgToChan(chan, ":" + u.prefix() + " PRIVMSG " + chan->name() + " :" + text, &u);
		}
		else if (target->size() > 0)
		{
			u.writeLine("MASKS NOT SUPPORTED YET");
		}
	}
	return (0);
}
