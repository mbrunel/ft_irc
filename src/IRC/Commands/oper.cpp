/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 15:32:22 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/20 16:30:15 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

int IrcServer::oper(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size() < 2)
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	Oper o = network.getOper(m.params()[0]);
	if (o.host.size() && o.host != u.socket()->host())
		return (writeNum(u, IrcError::nooperhost()));
	if (o.pass != m.params()[1])
		return (writeNum(u, IrcError::passwdmissmatch()));
	writeNum(u, IrcReply::youreoper());
	return true;
}
