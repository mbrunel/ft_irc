#include "IrcServer.hpp"
#include "Utils.hpp"

int IrcServer::lusers(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() && !Utils::match(m.params()[0], config.servername))
		return (writeNum(u, IRC::Error::nosuchserver(m.params()[0])));
	if (m.params().size() > 1 && !Utils::match(m.params()[1], config.servername))
		return (writeNum(u, IRC::Error::nosuchserver(m.params()[1])));
	Network::UserMap umap = network.users();
	Network::UserMap::const_iterator ium = umap.begin();
	writeNum(u, IRC::Reply::lusersclient(umap.size()));
	int nbOp = 0;
	int nbClients = 0;
	int nbUnknown = 0;
	while (ium != umap.end())
	{
		UserMode umode = ium->second->umode();
		UserRequirement ureq = ium->second->requirements();
		if (umode.isSet(UserMode::OPERATOR))
			nbOp++;
		if (ureq.flags())
			nbUnknown++;
		nbClients++;
		++ium;
	}
	writeNum(u, IRC::Reply::lusersop(nbOp));
	writeNum(u, IRC::Reply::lusersunknown(nbUnknown));
	writeNum(u, IRC::Reply::luserschannels(network.channels().size()));
	writeNum(u, IRC::Reply::lusersme(nbClients));
	return (0);
}

/*
Command : lusers
Params : None
*/
