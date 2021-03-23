/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 19:04:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 23:28:48 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BasicConnection.hpp"

BasicConnection::BasicConnection(Type type, TcpSocket *socket):_type(type), _socket(socket) {}

BasicConnection::~BasicConnection() {}

BasicConnection::Type BasicConnection::type() const { return (_type); }

TcpSocket *BasicConnection::socket() { return (_socket); }

void BasicConnection::writeTo(const std::string &msg) { _socket->writeLine(msg); }
