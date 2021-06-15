#include "IrcServer.hpp"

int IrcServer::privmsg(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (!m.params().size())
		return (writeNum(u, IRC::Error::norecipient(m.command())));
	if (m.params().size() == 1)
		return (writeNum(u, IRC::Error::notexttosend()));

	Params		targets(m.params()[0].split());
	const IRC::Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (target->isMask())
		{
			if (!u.umode().isSet(UserMode::OPERATOR))
				writeNum(u, IRC::Error::nosuchnick(target->c_str()));
			const std::string mask = target->mask().substr(1);
			if ((*target)[0] == '$')
			{
				if (Utils::match(mask, config.servername))
					network.msgToAll((IRC::MessageBuilder(u.prefix(), m.command()) << text).str(), &u);
				else
					writeNum(u, IRC::Error::badmask(*target));
			}
			else if ((*target)[0] == '#')
			{
				size_t dot;
				std::string toplevel;

				if ((dot = target->find_last_of('.')) == std::string::npos)
				{
					writeNum(u, IRC::Error::notoplevel(*target));
					continue ;
				}
				toplevel = target->substr(dot);
				if (toplevel.find('*') != std::string::npos || toplevel.find('?') != std::string::npos)
					writeNum(u, IRC::Error::wildtoplevel(*target));
				else
				{
					bool found = false;
					for (Network::UserMap::const_iterator it = network.users().begin(); it != network.users().end(); ++it)
						if (Utils::match(mask, it->second->socket()->host()))
						{
							it->second->writeLine((IRC::MessageBuilder(it->second->prefix(), m.command()) << text).str());
							found = true;
						}
					if (!found)
						writeNum(u, IRC::Error::badmask(*target));
				}
			}
		}
		else if (target->isNickname())
		{
			User *receiver = network.getByNickname(*target);
			if (!receiver)
			{
				writeNum(u, IRC::Error::nosuchnick(*target));
				continue ;
			}
			if (receiver->umode().isSet(UserMode::AWAY))
				writeNum(u, IRC::Reply::away(receiver->nickname(), receiver->awayReason()));
			receiver->writeLine((IRC::MessageBuilder(u.prefix(), m.command()) << text).str());
		}
		else if (target->isChannel())
		{
			Channel *chan = network.getByChannelname(*target);
			if (!chan)
				writeNum(u, IRC::Error::nosuchchannel(*target));
			else if (!chan->canSendToChannel(&u))
				writeNum(u, IRC::Error::cannotsendtochan(chan->name()));
			else
				chan->send((IRC::MessageBuilder(u.prefix(), m.command()) << chan->name() << text).str(), &u);
		}
	}
	u.idle() = ::time(NULL);
	return (0);
}
