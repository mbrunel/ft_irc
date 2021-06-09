/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lusers.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapapin <mapapin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 22:25:00 by mapapin           #+#    #+#             */
/*   Updated: 2021/06/08 22:25:02 by mapapin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

int     IrcServer::lusers(User &u, const Message &m)
{
	(void)m;
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
    Network::UserMap umap = network.users();
    Network::UserMap::const_iterator ium = umap.begin();
    writeNum(u, IrcReply::lusersclient(umap.size()));
    int nbOp = 0;
    int nbClients = 0;
	int nbUnknown = 0;
    while (ium != umap.end())
    {
        UserMode umode = ium->second->umode();
		UserRequirement ureq = ium->second->requirements();
        if (umode.isSet(UserMode::OPERATOR))
            nbOp++;
		if (ureq.flags())
			nbUnknown++;
        nbClients++;
        ++ium;
    }
    writeNum(u, IrcReply::lusersop(nbOp));
    writeNum(u, IrcReply::lusersunknown(nbUnknown));
	writeNum(u, IrcReply::luserschannels(network.channels().size()));
    writeNum(u, IrcReply::lusersme(nbClients));
    return (0);
}

/*
Command : lusers
Params : None
*/
