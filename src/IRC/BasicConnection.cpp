/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 19:04:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/21 17:47:59 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BasicConnection.hpp"

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

time_t &BasicConnection::clock()
{
	return _clock;
}

bool &BasicConnection::pongExpected()
{
	return _pongExpected;
}

BasicConnection::BasicConnection(TcpSocket *socket, Type type, unsigned hopcount) :
_socket(socket),
_type(type),
_hopcount(hopcount),
_clock(time(NULL)),
_pongExpected(false)
{}
