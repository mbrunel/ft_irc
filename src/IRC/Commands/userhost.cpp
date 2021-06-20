#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::userhost(User &u, const IRC::Message &m)
{

    if (!u.isRegistered())
        return (writeNum(u, IRC::Error::notregistered()));
    if (m.params().size() < 1)
        return (writeNum(u, IRC::Error::needmoreparams(m.command())));

    const Params params = m.params();

    for (unsigned int i = 0; i < params.size() && i < 5; i++)
    {
        User *us = network.getByNickname(params[i]);
        UserMode umode = us->umode();
		std::string msg;
        msg += params[i];
        msg += (umode.isSet(UserMode::OPERATOR) ? "*=" : "=");
        msg += (umode.isSet(UserMode::AWAY) ? "-" : "+");
        msg += us->username() + "@" + us->socket()->host();

        writeNum(u, IRC::Reply::userhostreply(msg));
    }
    
    return (0);
}
