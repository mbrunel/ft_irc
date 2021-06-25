#include "IrcServer.hpp"
#include "libft.hpp"

#define IRCSERVER_DEBUGLEVEL ""
#define IRCSERVER_COMMENTS "An obsolete IC server"

int IrcServer::version(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() && !ft::match(m.params()[0], config.servername))
		return (writeNum(u, IRC::Error::nosuchserver(m.params()[0])));
	writeNum(u, IRC::Reply::version(_version, IRCSERVER_DEBUGLEVEL, config.servername, IRCSERVER_COMMENTS));
	return (0);
}
