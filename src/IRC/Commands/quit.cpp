/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 06:25:41 by mbrunel           #+#    #+#             */
/*   Updated: 2021/06/03 06:40:50 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int IrcServer::quit(User &u, const Message &m)
{
	//need to send reason to everyone he knows
	(void)m;
	writeError(u.socket(), "QUIT msg received");
	disconnect(u.socket());
	return (0);
}
