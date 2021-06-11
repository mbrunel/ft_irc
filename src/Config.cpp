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

std::string Config::version()
{
	return (cfg->lookupString("ircserv", "version"));
}

std::string Config::servername()
{
	return (cfg->lookupString("ircserv", "servername"));
}

std::string Config::certFile()
{
	return (cfg->lookupString("ssl", "certfile", ""));
}

std::string Config::keyFile()
{
	return (cfg->lookupString("ssl", "keyfile", ""));
}

std::string Config::tcpPort()
{
	return (cfg->lookupString("tcp", "port"));
}

std::string Config::sslPort()
{
	return (cfg->lookupString("ssl", "port", 0));
}

bool Config::verbose()
{
	return (cfg->lookupBoolean("logs", "verbose", true));
}

std::string Config::logfile()
{
	return(cfg->lookupString("logs", "logfile", ""));
}

std::string Config::motdfile()
{
	return(cfg->lookupString("logs", "motdfile"));
}

int Config::maxConnections()
{
	return (cfg->lookupInt("ircserv", "maxconnections"));
}

int Config::maxChannels()
{
	return (cfg->lookupInt("ircserv", "maxchannels"));
}

std::map<std::string, Oper> Config::opers()
{
	std::map<std::string, Oper> opers;
	config4cpp::StringVector names;

	cfg->listLocallyScopedNames("", "", cfg->CFG_SCOPE, false, "uid-operator", names);
	for (int i = 0; i < names.length(); i++) {
		std::string login = cfg->lookupString(names[i], "login");
		std::string pass = cfg->lookupString(names[i], "password");
		std::string host = cfg->lookupString(names[i], "host", "");
		opers[login] = Oper(login, pass, host);
	}
	return opers;
}

time_t Config::ping()
{
	return (cfg->lookupDurationSeconds("ircserv", "ping_interval"));
}

time_t Config::pong()
{
	return (cfg->lookupDurationSeconds("ircserv", "ping_timeout"));
}

bool Config::floodControl()
{
	return (cfg->lookupBoolean("ircserver", "flood_control", "yes"));
}

void Config::usage() const
{
	std::cout << "Usage : ./ircserv <configuration file path>" << std::endl;
}
