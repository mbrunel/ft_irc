/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pingpong.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 17:53:40 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/24 19:28:49 by mapapin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::ping(User &u, const Message &m) //for when a client sends a ping : undefined in IRC
{
	if (!m.params().size())
		return (writeNum(u, IrcError::needmoreparams("PING")));
	u.writeLine("PONG " + m.params().front());
	return true;
}

int IrcServer::pong(User &u, const Message &m) //for when a client sends a pong : poorly defined in IRC
{
	if (!m.params().size())
		return (writeNum(u, IrcError::needmoreparams("PONG")));
	u.pongExpected() = false;
	::time(&u.clock());
	return true;
}
