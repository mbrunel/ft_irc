#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::ping(User &u, const Message &m) //for when a client sends a ping : undefined in IRC
{
	if (!m.params().size())
		return (writeNum(u, IrcError::needmoreparams("PING")));
	u.writeLine((MessageBuilder(config.servername, "PONG") << m.params()[0]).str());
	return true;
}

int IrcServer::pong(User &u, const Message &m) //for when a client sends a pong : poorly defined in IRC
{
	if (!m.params().size())
		return (writeNum(u, IrcError::needmoreparams("PONG")));
	u.pongExpected() = false;
	::time(&u.clock());
	return true;
}
