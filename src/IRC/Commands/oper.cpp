#include "IrcServer.hpp"
#include "MessageBuilder.hpp"
#include "ft.hpp"

int IrcServer::oper(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	Oper *o = network.getOper(m.params()[0]);
	if (!o || o->pass != m.params()[1])
		return (writeNum(u, IrcError::passwdmissmatch()));
	if (!ft::match(o->host, u.prefix()))
		return (writeNum(u, IrcError::nooperhost()));
	writeNum(u, IrcReply::youreoper());
	u.setUmode(u.umode().flags() | UserMode::OPERATOR);
	return (0);
}
