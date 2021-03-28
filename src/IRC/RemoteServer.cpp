/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RemoteServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 03:23:29 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/28 20:00:29 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RemoteServer.hpp"

RemoteServer::RemoteServer(TcpSocket *socket):
BasicConnection(socket, SERVER)
{}

RemoteServer::~RemoteServer() {}

const std::string &RemoteServer::name() const
{
	return _name;
}

void RemoteServer::setName(const std::string &name)
{
	_name = name;
}
