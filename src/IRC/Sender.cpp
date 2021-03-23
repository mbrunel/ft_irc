/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sender.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 19:04:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 21:21:47 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sender.hpp"

Sender::Sender(Type type, TcpSocket *socket):_type(type), _socket(socket) {}

Sender::~Sender() {}

Sender::Type Sender::type() const
{
	return (_type);
}

void Sender::writeTo(const std::string &msg) { _socket->writeLine(msg); }
