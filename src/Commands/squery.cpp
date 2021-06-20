#include "IrcServer.hpp"

int IrcServer::squery(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().empty())
		return (writeNum(u, IRC::Error::norecipient(m.command())));
	if (m.params().size() == 1)
		return (writeNum(u, IRC::Error::notexttosend()));
	const std::string &target = m.params()[0];
	User *receiver = network.getByServicename(target);
	if (!receiver)
		return (writeNum(u, IRC::Error::nosuchservice(target)));
	receiver->writeLine((IRC::MessageBuilder(u.prefix(), m.command()) << m.params()[1]).str());
	return (0);
}
