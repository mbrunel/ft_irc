#include "IrcServer.hpp"

int IrcServer::time(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
    // Change the 'if' below when the multi-server is managed
    if (m.params().size())
        return (writeNum(u, IRC::Error::nosuchserver(m.params()[0])));
    return (writeNum(u, IRC::Reply::time()));
}

/*
> Command : TIME
> Params : serveur target
*/
