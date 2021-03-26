/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcCommands.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 20:42:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/26 13:14:54 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::loadCmd(const std::string &cmd, BasicConnection::Type type, cmdType handler)
{
	commands[cmdIdType(cmd, type)] = handler;
}

void IrcServer::loadIrcCommands()
{
	loadCmd("AWAY", BasicConnection::USER, &IrcServer::away);
	loadCmd("JOIN", BasicConnection::USER, &IrcServer::join);
	loadCmd("NICK", BasicConnection::USER, &IrcServer::userNick);
	loadCmd("PRIVMSG", BasicConnection::USER, &IrcServer::privmsg);
	loadCmd("USER", BasicConnection::USER, &IrcServer::userUser);
}
