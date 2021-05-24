/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 11:30:18 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/21 16:10:25 by mbrunel          ###   ########.fr       */
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
	SslContext ctx(cfg.certFile().c_str(), cfg.keyFile().c_str());
	irc.setConfig(cfg);
	irc.listen(cfg.tcpPort().c_str());
	irc.listen(cfg.sslPort().c_str(), ctx.ctx());
	irc.run();
  } catch (std::exception &e) {
		irc.log() << e.what() << std::endl;
		return (1);
  } catch (config4cpp::ConfigurationException &e) {
		irc.log() << e.c_str() << std::endl;
		return (1);
	}
	return (0);
}
