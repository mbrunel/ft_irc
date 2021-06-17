#include "IrcServer.hpp"

int IrcServer::service(User &u, const IRC::Message &m)
{
	if (u.isRegistered() || !u.requirements().areSet(UserRequirement::NICK | UserRequirement::USER))
		return (writeNum(u, IRC::Error::alreadyregistred()));
	if (m.params().size() < 6)
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	const IRC::Param &nick = m.params()[0];
	if (!nick.isNickname())
		return (writeNum(u, IRC::Error::erroneusnickname(nick)));
	if (network.getByNickname(nick) || network.isFnick(nick))
		return (writeNum(u, IRC::Error::nicknameinuse(nick)));
	if (!Utils::match(m.params()[2], config.servername))
		return (writeNum(u, IRC::Error::nosuchserver(m.params()[2])));
	if (u.requirements().isSet(UserRequirement::PASS))
	{
		writeNum(u, IRC::Error::passwdmissmatch());
		disconnect(u, "Bad Password");
		return (-1);
	}
	network.remove(&u);
	u.setType(User::SERVICE);
	u.setNickname(nick);
	u.setRealname(m.params()[5]);
	network.add(&u);
	u.unsetRequirement(UserRequirement::NICK);
	u.unsetRequirement(UserRequirement::USER);
	writeWelcome(u);
	return (0);
}