/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:07:27 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/21 15:31:23 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <locale.h>
#include <iostream>
#include <stdlib.h>
#include <map>
#include <time.h>
#include "config4cpp/Configuration.h"

typedef config4cpp::Configuration Configuration;

struct Oper
{
	Oper();
	Oper(std::string login, std::string pass, std::string host);
	std::string login;
	std::string pass;
	std::string host;
};

class Config
{
  public:
	Config(int ac, char **av);
	void usage() const;
	std::string version();
	std::string servername();
	std::string certFile();
	std::string keyFile();
	std::string tcpPort();
	std::string sslPort();
	std::string logfile();
	time_t ping();
	time_t pong();
	bool floodControl();
	int maxConnections();
	int maxChannels();
	bool verbose();
	std::map<std::string, Oper> opers();

  private:
	Configuration *cfg;
};
