#include "IrcServer.hpp"
#include "MessageBuilder.hpp"
#include "ft.hpp"

int IrcServer::privmsg(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IrcError::norecipient(m.command())));
	if (m.params().size() == 1)
		return (writeNum(u, IrcError::notexttosend()));

	Params		targets(m.params()[0].split());
	const Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (target->isMask())
		{
			if (!u.umode().isSet(UserMode::OPERATOR))
				writeNum(u, IrcError::nosuchnick(target->c_str()));
			const std::string mask = target->mask().substr(1);
			if ((*target)[0] == '$')
			{
				if (ft::match(mask, config.servername))
					network.msgToAll((MessageBuilder(u.prefix(), m.command()) << text).str(), &u);
				else
					writeNum(u, IrcError::badmask(*target));
			}
			else if ((*target)[0] == '#')
			{
				size_t dot;
				std::string toplevel;

				if ((dot = target->find_last_of('.')) == std::string::npos)
				{
					writeNum(u, IrcError::notoplevel(*target));
					continue ;
				}
				toplevel = target->substr(dot);
				if (toplevel.find('*') != std::string::npos || toplevel.find('?') != std::string::npos)
					writeNum(u, IrcError::wildtoplevel(*target));
				else
				{
					bool found = false;
					for (Network::UserMap::const_iterator it = network.users().begin(); it != network.users().end(); ++it)
						if (ft::match(mask, it->second->socket()->host()))
						{
							it->second->writeLine((MessageBuilder(it->second->prefix(), m.command()) << text).str());
							found = true;
						}
					if (!found)
						writeNum(u, IrcError::badmask(*target));
				}
			}
		}
		else if (target->isNickname())
		{
			User *receiver = network.getByNickname(*target);
			if (!receiver)
			{
				writeNum(u, IrcError::nosuchnick(*target));
				continue ;
			}
			if (receiver->umode().isSet(UserMode::AWAY))
				writeNum(u, IrcReply::away(receiver->nickname(), receiver->awayReason()));
			receiver->writeLine((MessageBuilder(u.prefix(), m.command()) << text).str());
		}
		else if (target->isChannel())
		{
			Channel *chan = network.getByChannelname(*target);
			if (!chan)
				writeNum(u, IrcError::nosuchchannel(*target));
			else if (!chan->canSendToChannel(&u))
				writeNum(u, IrcError::cannotsendtochan(chan->name()));
			else
				chan->send((MessageBuilder(u.prefix(), m.command()) << chan->name() << text).str(), &u);
		}
	}
	u.idle() = ::time(NULL);
	return (0);
}
