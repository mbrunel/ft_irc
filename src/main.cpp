/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 11:30:18 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 19:45:44 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include <iostream>

int main()
{
	IrcServer irc;
	irc.setMaxConnections(5);
	irc.setVerbose(true);
	//irc.setLogDestination("logfile.txt");
	irc.loadIrcCommands();
  try {
	SslContext ctx("ircserv.ssl.crt", "ircserv.ssl.key");
	irc.listen("6667");
	irc.listen("6697", ctx.ctx());
	irc.run();
  } catch (std::exception &e) {
		irc.log() << e.what() << std::endl;
		return (1);
	}
	return (0);
}
