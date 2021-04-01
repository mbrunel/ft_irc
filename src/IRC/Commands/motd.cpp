/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 13:41:15 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 16:33:06 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"
#include "IrcServer.hpp"

void IrcServer::motd(User &u, const Message &m)
{
	if (!u.isRegistered())
		u.writeLine(IrcError::notregistered(prefix));
	if (m.params().size() > 0 && m.params()[0] != prefix)
	{
		RemoteServer *r(network.getByServername(m.params()[0]));
		if (r)
			r->writeLine(m.entry());
		return ;
	}
	writeMotd(u);
}
