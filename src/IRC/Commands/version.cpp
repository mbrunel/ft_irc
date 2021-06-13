#include "IrcServer.hpp"

#define IRCSERVER_VERSION "1.0.0"
#define IRCSERVER_DEBUGLEVEL ""
#define IRCSERVER_COMMENTS "An obsolete IC server (please do not use it in real life)"

int IrcServer::version(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() && m.params()[0] != config.servername)
		return (writeNum(u, IrcError::nosuchserver(m.params()[0])));
	writeNum(u, IrcReply::version(IRCSERVER_VERSION, IRCSERVER_DEBUGLEVEL, config.servername, IRCSERVER_COMMENTS));
	return (0);
}
