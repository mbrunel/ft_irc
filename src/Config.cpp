/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:23:02 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/20 15:57:45 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	return (cfg->lookupString("ircserv", "tcpport"));
}

std::string Config::sslPort()
{
	return (cfg->lookupString("ircserv", "sslport", 0));
}

bool Config::verbose()
{
	return (cfg->lookupBoolean("logs", "verbose", true));
}

std::string Config::logfile()
{
	return(cfg->lookupString("logs", "logfile", ""));
}

int Config::maxConnections()
{
	return (cfg->lookupInt("ircserv", "maxconnections"));
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

void Config::usage() const
{
	std::cout << "Usage : ./ircserv <configuration file path>" << std::endl;
}
