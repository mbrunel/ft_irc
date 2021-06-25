#include "IrcServer.hpp"
#include "libft.hpp"

int IrcServer::oper(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	IrcServerConfig::OperMap::const_iterator io = config.opers.find(m.params()[0]);
	if (io == config.opers.end() || io->second.pass != m.params()[1])
		return (writeNum(u, IRC::Error::passwdmissmatch()));
	if (!ft::match(io->second.host, u.prefix()))
		return (writeNum(u, IRC::Error::nooperhost()));
	writeNum(u, IRC::Reply::youreoper());
	u.setUmode(u.umode().flags() | UserMode::OPERATOR);
	return (0);
}
