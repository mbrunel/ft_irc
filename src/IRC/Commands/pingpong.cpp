#include "IrcServer.hpp"

int IrcServer::ping(User &u, const IRC::Message &m) //for when a client sends a ping : undefined in IRC
{
	if (!m.params().size())
		return (writeNum(u, IRC::Error::needmoreparams("PING")));
	u.writeLine((IRC::MessageBuilder(config.servername, "PONG") << m.params()[0]).str());
	return true;
}

int IrcServer::pong(User &u, const IRC::Message &m) //for when a client sends a pong : poorly defined in IRC
{
	if (!m.params().size())
		return (writeNum(u, IRC::Error::needmoreparams("PONG")));
	u.pongExpected() = false;
	::time(&u.clock());
	return true;
}
