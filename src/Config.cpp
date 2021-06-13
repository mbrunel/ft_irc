#include "Config.hpp"

Oper::Oper(){}

Oper::Oper(std::string login, std::string pass, std::string host):login(login), pass(pass), host(host){}

Config::Config(int ac, char **av)
{
	std::string configFile;

	if (ac == 1)
		configFile = "ircserv.cfg";
	else if (ac == 2)
		configFile = av[1];
	else if (ac > 2)
	{
		usage();
		exit(1);
	}
	cfg = Configuration::create();
	cfg->parse(Configuration::INPUT_FILE, configFile.c_str());
}

void Config::servername(std::string &n)
{
	n = cfg->lookupString(IRC_SCOPE, "servername");
}

std::string Config::certFile()
{
	return cfg->lookupString(SERVER_SCOPE, "ssl-certificat", "");
}

std::string Config::keyFile()
{
	return cfg->lookupString(SERVER_SCOPE, "ssl-key", "");
}

std::string Config::tcpPort()
{
	return cfg->lookupString(SERVER_SCOPE, "tcp-port");
}

std::string Config::sslPort()
{
	return cfg->lookupString(SERVER_SCOPE, "ssl-port", 0);
}

bool Config::verbose()
{
	return (cfg->lookupBoolean(SERVER_SCOPE, "logs.verbose", true));
}

std::string Config::logfile()
{
	return cfg->lookupString(SERVER_SCOPE, "logs.logfile", "");
}

void Config::motdfile(std::string &m)
{
	m = cfg->lookupString(IRC_SCOPE, "motdfile");
}

int Config::maxConnections()
{
	return (cfg->lookupInt(IRC_SCOPE, "limits.maxconnections", 1080));
}

int Config::maxChannels()
{
	return (cfg->lookupInt(IRC_SCOPE, "limits.maxchannels", 1080));
}

int Config::maxMasks()
{
	return (cfg->lookupInt(IRC_SCOPE, "limits.maxchanmasks", 32));
}

void Config::opers(std::map<std::string, Oper> &o)
{
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

time_t Config::ping()
{
	return (cfg->lookupDurationSeconds(IRC_SCOPE, "limits.ping_interval", 100));
}

time_t Config::pong()
{
	return (cfg->lookupDurationSeconds(IRC_SCOPE, "limits.ping_timeout", 100));
}

bool Config::floodControl()
{
	return (cfg->lookupBoolean(IRC_SCOPE, "limits.flood_control", "yes"));
}

void Config::fnicks(std::set<std::string> &f)
{
	config4cpp::StringVector nicks;

	cfg->lookupList(IRC_SCOPE, "banned_nicks", nicks);
	f = std::set<std::string>(nicks.c_array(), nicks.c_array() + nicks.length());
}

void Config::usage() const
{
	std::cout << "Usage : ./ircserv <configuration file path>" << std::endl;
}

