/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:22:52 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/20 18:05:37 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int IrcServer::away(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().empty())
	{
		writeNum(u, IrcReply::unaway());
		u.setUmode(UserMode(u.umode().flags() & ~UserMode::AWAY));
	}
	else
	{
		writeNum(u, IrcReply::nowaway());
		u.setUmode(UserMode(u.umode().flags() | UserMode::AWAY));
		u.setAway(m.params()[0]);
	}
	return (0);
}
