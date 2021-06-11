#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::oper(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	Oper *o = network.getOper(m.params()[0]);
	if (!o || o->pass != m.params()[1])
		return (writeNum(u, IrcError::passwdmissmatch()));
	if (o->host.size() && o->host != u.socket()->host())
		return (writeNum(u, IrcError::nooperhost()));
	writeNum(u, IrcReply::youreoper());
	u.setUmode(UserMode(u.umode().flags() | UserMode::OPERATOR));
	return true;
}
