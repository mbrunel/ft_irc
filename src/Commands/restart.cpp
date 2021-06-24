#include "IrcServer.hpp"

int IrcServer::restart(User &u, const IRC::Message &m)
{
	(void)m;
	if (!u.isRegistered())
		return writeNum(u, IRC::Error::notregistered());
	if (!u.umode().isSet(UserMode::OPERATOR))
		return writeNum(u, IRC::Error::noprivileges());
	_state = RESTART;
	log() << "RESTARTING" << std::endl;
	return 0;
}
