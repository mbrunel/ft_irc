/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:41:15 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/03 14:37:22 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

int IrcServer::motd(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() > 0 && m.params()[0] != prefix)
	{
		Server *s(network.getByServername(m.params()[0]));
		if (s)
			s->writeLine(m.entry());
	}
	else
		writeMotd(u);
	return (0);
}
