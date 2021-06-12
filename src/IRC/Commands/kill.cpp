#include "IrcServer.hpp"

int IrcServer::kill(User &u, const Message &m)
{
	User *target;

	if (m.params().size() < 2)
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	if (!u.umode().isSet(UserMode::OPERATOR|UserMode::LOCAL_OPERATOR))
		return (writeNum(u, IrcError::noprivileges()));
	if (!(target = network.getByNickname(m.params()[0])))
		return (writeNum(u, IrcError::nosuchnick(m.params()[0])));
	network.addFnick(target->nickname());
	disconnect(*target, "killed by " + u.prefix() + " : " + m.params()[1]);
	return (0);
}
