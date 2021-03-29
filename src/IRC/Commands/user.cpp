/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:13:59 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/29 18:41:17 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcError.hpp"
#include "IrcServer.hpp"

void IrcServer::user(User &u, const Message &msg)
{
	if (!u.requirements().isSet(UserRequirement::USER))
	{
		u.writeLine(IrcError::alreadyregistred("127.0.0.1"));
		return ;
	}
	(void)msg;
	u.unsetRequirement(UserRequirement::USER);
	if (u.isRegistered())
		u.writeLine("THIS IS THE MOTD");
}
