#include "IrcServer.hpp"

int IrcServer::rehash(User &u, const IRC::Message &m)
{
	(void)m;
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (!u.umode().isSet(UserMode::OPERATOR))
		return (writeNum(u, IRC::Error::noprivileges()));
	try { loadConfig(config.configfile); }
	catch (config4cpp::ConfigurationException &e)
	{
		log() << e.c_str() << std::endl;
		return 1;
	}
	writeNum(u, IRC::Reply::rehashing(config.configfile));
	log() << "REHASHING " << config.configfile << std::endl;
	return 0;
}


