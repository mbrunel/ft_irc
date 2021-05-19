/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/26 12:07:27 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/19 16:19:03 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <locale.h>
#include <iostream>
#include <stdlib.h>
#include "config4cpp/Configuration.h"

typedef config4cpp::Configuration Configuration;

class Config
{
  public:
	Config(int ac, char **av);
	void usage() const;
	std::string certFile();
	std::string keyFile();
	std::string tcpPort();
	std::string sslPort();
	bool verbose();
	std::string logfile();
	int maxConnections();

  private:
	Configuration *cfg;
};
