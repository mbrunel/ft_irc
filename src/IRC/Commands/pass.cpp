#include "IrcServer.hpp"

int IrcServer::pass(User &u, const IRC::Message &m)
{
	if (u.isRegistered())
		return writeNum(u, IRC::Error::alreadyregistred());
	if (!m.params().size())
		return writeNum(u, IRC::Error::needmoreparams(m.command()));
	if (!config.pass.size() || m.params()[0] == config.pass)
		u.unsetRequirement(UserRequirement::PASS);
	else
		u.setRequirement(UserRequirement::PASS);
	return 0;
}
