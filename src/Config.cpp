#include "Config.hpp"

Oper::Oper(){}

Oper::Oper(std::string login, std::string pass, std::string host):login(login), pass(pass), host(host){}

Config::Config(const std::string &file)
{
	cfg = Configuration::create();
	try { cfg->parse(Configuration::INPUT_FILE, file.c_str()); }
	catch (config4cpp::ConfigurationException &e)
	{
		cfg->destroy();
		throw ;
	}
}

Config::~Config() { cfg->destroy(); }

std::string Config::configfile()
{
	return cfg->fileName();
}

std::string Config::tcpPort()
{
	return cfg->lookupString(SERVER_SCOPE, "tcp-port");
}

std::string Config::sslPort()
{
	return cfg->lookupString(SERVER_SCOPE, "ssl-port", "");
}

std::string Config::certFile()
{
	return cfg->lookupString(SERVER_SCOPE, "ssl-certificat", "");
}

std::string Config::keyFile()
{
	return cfg->lookupString(SERVER_SCOPE, "ssl-key", "");
}

std::string Config::logfile()
{
	return cfg->lookupString(SERVER_SCOPE, "logfile", "");
}

std::string Config::servername()
{
	return cfg->lookupString(IRC_SCOPE, "servername");
}

std::string Config::serverpass()
{
	return cfg->lookupString(IRC_SCOPE, "serverpass", "");
}

std::string Config::shortinfo()
{
	return cfg->lookupString(IRC_SCOPE, "shortinfo");
}

std::string Config::motdfile()
{
	return cfg->lookupString(IRC_SCOPE, "motdfile");
}

int Config::maxConnections()
{
	return (cfg->lookupInt(IRC_SCOPE, "limits.maxconnections"));
}

int Config::maxChannels()
{
	return (cfg->lookupInt(IRC_SCOPE, "limits.maxchannels"));
}

int Config::maxMasks()
{
	return (cfg->lookupInt(IRC_SCOPE, "limits.maxchanmasks"));
}

size_t Config::historySize()
{
	return (cfg->lookupInt(IRC_SCOPE, "limits.history_size"));
}

time_t Config::ping()
{
	return (cfg->lookupDurationSeconds(IRC_SCOPE, "limits.ping_interval"));
}

time_t Config::pong()
{
	return (cfg->lookupDurationSeconds(IRC_SCOPE, "limits.pong_timeout"));
}

bool Config::floodControl()
{
	return (cfg->lookupBoolean(IRC_SCOPE, "limits.flood_control"));
}

void Config::opers(std::map<std::string, Oper> &o)
{
	o.clear();
	config4cpp::StringVector names;
	config4cpp::StringBuffer filterPattern;

	Configuration::mergeNames(IRC_SCOPE, "uid-operator", filterPattern);
	cfg->listFullyScopedNames(IRC_SCOPE, "", cfg->CFG_SCOPE, true, filterPattern.c_str(), names);
	for (int i = 0; i < names.length(); i++) {
		std::string login = cfg->lookupString(names[i], "login");
		std::string pass = cfg->lookupString(names[i], "password");
		std::string host = cfg->lookupString(names[i], "host", "*");
		o[login] = Oper(login, pass, host);
	}
}

void Config::fnicks(std::set<std::string> &f)
{
	f.clear();
	config4cpp::StringVector nicks;

	cfg->lookupList(IRC_SCOPE, "banned_nicks", nicks);
	f = std::set<std::string>(nicks.c_array(), nicks.c_array() + nicks.length());
}
