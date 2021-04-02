/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:41:15 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/02 05:10:26 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

void IrcServer::motd(User &u, const Message &m)
{
	if (!u.isRegistered())
		writeNum(u, IrcError::notregistered());
	if (m.params().size() > 0 && m.params()[0] != prefix)
	{
		Server *s(network.getByServername(m.params()[0]));
		if (s)
			s->writeLine(m.entry());
		return ;
	}
	writeMotd(u);
}
