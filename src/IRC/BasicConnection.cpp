/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 19:04:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/28 15:27:35 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BasicConnection.hpp"

BasicConnection::BasicConnection(TcpSocket *socket, Type type, unsigned hopcount) :
_type(type),
_socket(socket),
_hopcount(hopcount)
{}

BasicConnection::~BasicConnection()
{}

BasicConnection::Type BasicConnection::type() const
{
	return (_type);
}

TcpSocket *BasicConnection::socket() const
{
	return (_socket);
}

unsigned BasicConnection::hopcount() const
{
	return (_hopcount);
}

void BasicConnection::writeLine(const std::string &content)
{
	_socket->writeLine(content);
}
