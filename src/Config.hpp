/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:07:27 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/20 13:19:21 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <locale.h>
#include <iostream>
#include <stdlib.h>
#include <map>
#include "config4cpp/Configuration.h"

typedef config4cpp::Configuration Configuration;

struct Oper
{
	Oper();
	Oper(std::string login, std::string pass);
	std::string login;
	std::string pass;
	bool registered;
};

class Config
{
  public:
	Config(int ac, char **av);
	void usage() const;
	std::string certFile();
	std::string keyFile();
	std::string tcpPort();
	std::string sslPort();
	std::string logfile();
	int maxConnections();
	bool verbose();
	std::map<std::string, Oper> opers();

  private:
	Configuration *cfg;
};
