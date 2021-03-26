/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 19:04:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/26 13:28:50 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BasicConnection.hpp"

BasicConnection::BasicConnection(Type type, TcpSocket *socket):_type(type), _socket(socket) {}

BasicConnection::~BasicConnection() {}

BasicConnection::Type BasicConnection::type() const { return (_type); }

TcpSocket *BasicConnection::socket() { return (_socket); }

void BasicConnection::writeLine(const std::string &msg) { _socket->writeLine(msg); }
