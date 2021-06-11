#include "IrcServer.hpp"

int IrcServer::time(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
    // Change the 'if' below when the multi-server is managed
    if (m.params().size())
        return (writeNum(u, IrcError::nosuchserver(m.params()[0])));
    return (writeNum(u, IrcReply::time()));
}

/*
> Command : TIME
> Params : serveur target
*/
