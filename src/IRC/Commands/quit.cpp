/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 06:25:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/06/04 17:09:19 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int IrcServer::quit(User &u, const Message &m)
{
	if (!u.isRegistered() || m.params().empty())
		disconnect(u, "Client Quit", true);
	else
		disconnect(u, m.params()[0], true);
	return (0);
}
