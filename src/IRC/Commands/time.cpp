#include "IrcServer.hpp"
#include "Utils.hpp"

int IrcServer::time(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() && !Utils::match(m.params()[0], config.servername))
		return (writeNum(u, IRC::Error::nosuchserver(m.params()[0])));
	return (writeNum(u, IRC::Reply::time()));
}

/*
> Command : TIME
> Params : serveur target
*/
