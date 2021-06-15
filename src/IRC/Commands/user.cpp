#include "IrcServer.hpp"

int IrcServer::user(User &u, const Message &m)
{
	if (!u.requirements().isSet(UserRequirement::USER))
		return (writeNum(u, IrcError::alreadyregistred()));
	else if (m.params().size() < 4)
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	u.setUsername(m.params()[0]);
	u.setUmode(strtoul(m.params()[1].c_str(), NULL, 10) & (UserMode::INVISIBLE | UserMode::WALLOPS | UserMode::RESTRICTED));
	u.setRealname(m.params()[3]);
	u.unsetRequirement(UserRequirement::USER);
	if (u.isRegistered())
	{
		writeWelcome(u);
		network.addNickToHistory(u);
	}
	else if (u.requirements().flags() == UserRequirement::PASS)
	{
		writeNum(u, IrcError::passwdmissmatch());
		disconnect(u, "Bad Password");
	}
	return (0);
}
