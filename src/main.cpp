/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 11:30:18 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/20 14:52:41 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "Config.hpp"
#include <iostream>

int main(int ac, char **av)
{
	IrcServer irc;
  try {
	Config cfg(ac, av);
	irc.setMaxConnections(cfg.maxConnections());
	irc.setVerbose(cfg.verbose());
	irc.setLogDestination(cfg.logfile());
	SslContext ctx(cfg.certFile().c_str(), cfg.keyFile().c_str());
	irc.listen(cfg.tcpPort().c_str());
	irc.listen(cfg.sslPort().c_str(), ctx.ctx());
	irc.setOpers(cfg.opers());
	irc.run();
  } catch (std::exception &e) {
		irc.log() << e.what() << std::endl;
		return (1);
	}
	catch (config4cpp::ConfigurationException &e)
	{
		irc.log() << e.c_str() << std::endl;
		return (2);
	}
	return (0);
}
