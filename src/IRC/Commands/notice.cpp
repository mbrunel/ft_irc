#include "IrcServer.hpp"

int IrcServer::notice(User &u, const IRC::Message &m)
{
	if (!u.isRegistered() || m.params().size() < 2)
		return (1);

	Params				targets(m.params()[0].split());
	const IRC::Param	&text(m.params()[1]);

	for (Params::const_iterator target = targets.begin(); target != targets.end(); ++target)
	{
		if (target->isMask())
		{
			if (!u.umode().isSet(UserMode::OPERATOR))
				continue ;
			const std::string mask = target->mask().substr(1);
			if ((*target)[0] == '$')
			{
				if (Utils::match(mask, config.servername))
					network.msgToAll((IRC::MessageBuilder(u.prefix(), m.command()) << text).str(), &u);
			}
			else if ((*target)[0] == '#')
			{
				size_t dot;
				std::string toplevel;

				if ((dot = target->find_last_of('.')) == std::string::npos)
					continue ;
				toplevel = target->substr(dot);
				if (toplevel.find('*') == std::string::npos && toplevel.find('?') == std::string::npos)
					for (Network::UserMap::const_iterator it = network.users().begin(); it != network.users().end(); ++it)
						if (Utils::match(mask, it->second->socket()->host()))
							it->second->writeLine((IRC::MessageBuilder(it->second->prefix(), m.command()) << text).str());
			}
		}
		if (target->isNickname())
		{
			User *receiver = network.getByNickname(*target);
			if (receiver)
				receiver->writeLine((IRC::MessageBuilder(u.prefix(), m.command()) << text).str());
		}
		else if (target->isChannel())
		{
			Channel *chan = network.getByChannelname(*target);
			if (chan && chan->canSendToChannel(&u))
				chan->send((IRC::MessageBuilder(u.prefix(), m.command()) << chan->name() << text).str(), &u);
		}
	}
	u.idle() = ::time(NULL);
	return (0);
}
