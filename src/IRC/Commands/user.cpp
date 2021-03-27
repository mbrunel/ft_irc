/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:13:59 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/27 14:46:29 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include "IrcServer.hpp"

void IrcServer::userUser(BasicConnection *sender, const Message &msg)
{
	User *user;

	if (!(user = userFromConnection(sender)))
		return ;
	if (user->state() == User::CONNECTED)
	{
		user->writeLine(IrcError::alreadyregistred("127.0.0.1"));
		return ;
	}
	(void)msg;
	// Just so it exists
	user->setState(User::CONNECTED);
	user->writeLine("THIS IS THE MOTD");
}
