/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:13:59 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/03 14:39:13 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int IrcServer::user(User &u, const Message &m)
{
	if (!u.requirements().isSet(UserRequirement::USER))
		return (writeNum(u, IrcError::alreadyregistred()));
	else if (m.params().size() < 4)
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	u.setUsername(m.params()[0]);
	// Add mode
	u.setRealname(m.params()[3]);
	u.unsetRequirement(UserRequirement::USER);
	if (u.isRegistered())
		writeWelcome(u);
	return (0);
}
