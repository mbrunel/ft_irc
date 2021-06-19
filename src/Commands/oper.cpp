#include "IrcServer.hpp"
#include "libft.hpp"

int IrcServer::oper(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	Oper *o = network.getOper(m.params()[0]);
	if (!o || o->pass != m.params()[1])
		return (writeNum(u, IRC::Error::passwdmissmatch()));
	if (!ft::match(o->host, u.prefix()))
		return (writeNum(u, IRC::Error::nooperhost()));
	writeNum(u, IRC::Reply::youreoper());
	u.setUmode(u.umode().flags() | UserMode::OPERATOR);
	return (0);
}
