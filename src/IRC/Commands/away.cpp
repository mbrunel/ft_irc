/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:22:52 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 15:11:31 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::away(BasicConnection *sender, const Message &m)
{
	User *u;

	if (!(u = userFromConnection(sender)))
		return ;
	(void)m;
}
