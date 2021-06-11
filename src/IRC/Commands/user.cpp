#include "IrcServer.hpp"

int IrcServer::user(User &u, const Message &m)
{
	if (!u.requirements().isSet(UserRequirement::USER))
		return (writeNum(u, IrcError::alreadyregistred()));
	else if (m.params().size() < 4)
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	u.setUsername(m.params()[0]);
	// Add mode
	u.setRealname(m.params()[3]);
	u.unsetRequirement(UserRequirement::USER);
	if (u.isRegistered())
		writeWelcome(u);
	return (0);
}
