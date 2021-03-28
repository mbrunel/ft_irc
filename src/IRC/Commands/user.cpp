/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:13:59 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/28 21:08:23 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include "IrcServer.hpp"

void IrcServer::unknownUser(BasicConnection *sender, const Message &msg)
{
	User *usr = network.newUser(sender);
	delete sender;
	network.addConnection(usr);
	usr->setState(User::HAS_USER);
	(void)msg;
}

void IrcServer::userUser(BasicConnection *sender, const Message &msg)
{
	User *user = network.conToUsr(sender);

	assert(user->state() != User::HAS_NOTHING);
	if (user->state() != User::HAS_NICK)
	{
		user->writeLine(IrcError::alreadyregistred("127.0.0.1"));
		return ;
	}
	(void)msg;
	// Just so it exists
	network.addUser(user);
	user->setState(User::REGISTERED);
	user->writeLine("THIS IS THE MOTD");
}
