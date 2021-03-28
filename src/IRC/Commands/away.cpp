/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:22:52 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/28 19:41:58 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::away(BasicConnection *sender, const Message &m)
{
	User *u;

	u = network.conToUsr(sender);
	(void)u;
	(void)m;
}
