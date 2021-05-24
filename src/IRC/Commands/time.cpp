/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapapin <mapapin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/24 17:07:05 by mapapin           #+#    #+#             */
/*   Updated: 2021/05/24 17:07:09 by mapapin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int IrcServer::time(User &u, const Message &m)
{
    // Change the 'if' below when the multi-server is managed
    if (!!m.params().size())
        return (writeNum(u, IrcError::nosuchserver(m.params()[0])));
    return (writeNum(u, IrcReply::time()));
}

/*
> Command : TIME
> Params : serveur target
*/
