#include "IrcServer.hpp"
#include "ft.hpp"
#include "MessageBuilder.hpp"

int IrcServer::pass(User &u, const Message &m)
{
	if (u.isRegistered())
		return writeNum(u, IrcError::alreadyregistred());
	if (!m.params().size())
		return writeNum(u, IrcError::needmoreparams(m.command()));
	if (!config.pass.size() || m.params()[0] == config.pass)
		u.unsetRequirement(UserRequirement::PASS);
	else
		u.setRequirement(UserRequirement::PASS);
	return 0;
}
