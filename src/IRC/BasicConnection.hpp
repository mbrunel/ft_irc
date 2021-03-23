/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 18:50:36 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 23:28:05 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "TcpSocket.hpp"

// class that allows to interact with an undefined connection

class BasicConnection
{
  public:
	enum Type {USER, SERVER};

	BasicConnection(Type type, TcpSocket *socket);
	virtual ~BasicConnection();

	Type type() const;
	TcpSocket *socket();
	void writeTo(const std::string &);

  protected:
	Type		_type;
	TcpSocket	*_socket;
};
