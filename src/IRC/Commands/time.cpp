/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapapin <mapapin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 17:07:05 by mapapin           #+#    #+#             */
/*   Updated: 2021/05/31 16:07:10 by mapapin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int IrcServer::time(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
    // Change the 'if' below when the multi-server is managed
    if (m.params().size())
        return (writeNum(u, IrcError::nosuchserver(m.params()[0])));
    return (writeNum(u, IrcReply::time()));
}

/*
> Command : TIME
> Params : serveur target
*/
