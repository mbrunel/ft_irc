/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   away.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 13:22:52 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 13:29:26 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

void IrcServer::away(BasicConnection *sender, const Message &msg)
{
	User *user;

	if (!(user = userFromConnection(sender)))
		return ;
	(void)msg;
}
