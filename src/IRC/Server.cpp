/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 21:05:17 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/01 22:21:23 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(TcpSocket *socket):
BasicConnection(socket, SERVER)
{}

Server::~Server() {}

const std::string &Server::name() const
{
	return _name;
}

void Server::setName(const std::string &name)
{
	_name = name;
}
