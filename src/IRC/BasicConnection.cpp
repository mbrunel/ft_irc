/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 19:04:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/29 18:12:10 by asoursou         ###   ########.fr       */
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

BasicConnection::BasicConnection(TcpSocket *socket, Type type, unsigned hopcount) :
_socket(socket),
_type(type),
_hopcount(hopcount)
{}
