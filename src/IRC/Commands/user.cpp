/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:13:59 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 18:10:48 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

static bool check(const std::string &prefix, User &u, const Message &m)
{
	if (!u.requirements().isSet(UserRequirement::USER))
		u.writeNum(prefix, IrcError::alreadyregistred());
	else if (m.params().size() < 4)
		u.writeNum(prefix, IrcError::needmoreparams(m.command()));
	else
		return true;
	return false;
}

void IrcServer::user(User &u, const Message &m)
{
	if (!check(prefix, u, m))
		return ;
	u.setUsername(m.params()[0]);
	// Add mode
	u.setRealname(m.params()[3]);
	u.unsetRequirement(UserRequirement::USER);
	if (u.isRegistered())
		writeWelcome(u);
}
