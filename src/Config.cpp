/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:23:02 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/19 16:30:04 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

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
	return (cfg->lookupString("ssl", "certfile"));
}

std::string Config::keyFile()
{
	return (cfg->lookupString("ssl", "keyfile"));
}

std::string Config::tcpPort()
{
	return (cfg->lookupString("ircserv", "tcpport"));
}

std::string Config::sslPort()
{
	return (cfg->lookupString("ircserv", "sslport"));
}

bool Config::verbose()
{
	return (cfg->lookupBoolean("logs", "verbose"));
}

std::string Config::logfile()
{
	return(cfg->lookupString("logs", "logfile"));
}

int Config::maxConnections()
{
	return (cfg->lookupInt("ircserv", "maxconnections"));
}

void Config::usage() const
{
	std::cout << "Usage : ./ircserv <configuration file path>" << std::endl;
}
