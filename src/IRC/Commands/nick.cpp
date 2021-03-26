/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:14:24 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 13:29:16 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::userNick(BasicConnection *sender, const Message &msg)
{
	User *user;
	if (!(user = userFromConnection(sender)))
		return ;
	if (msg.params().empty())
	{
		sender->writeLine("No nick given\n");
		return ;
	}
	std::string nickname = Param(msg.params().back()).asNickname();
	if (nickname.empty())
	{
		sender->writeLine("erroneus nickname\n");
		return ;
	}
	if (allUsers.find(nickname) != allUsers.end())
	{
		sender->writeLine("nickname already in use\n");
		return ;
	}
	if (user->state() != User::NEED_NICK)
		allUsers.erase(user->nickname());

	// Incomplete / ugly error handling ofc
	user->setNickname(nickname);
	if (user->state() == User::NEED_NICK)
		user->setState(User::NEED_USER);
	allUsers[nickname] = user;
}
