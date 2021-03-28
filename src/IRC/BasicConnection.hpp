/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 18:50:36 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/28 15:27:14 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "TcpSocket.hpp"

// class that allows to interact with an undefined connection

class BasicConnection
{
  public:
	enum Type
	{
		SERVER,
		SERVICE,
		UNKNOWN,
		USER
	};

	BasicConnection(TcpSocket *socket, Type type = UNKNOWN, unsigned hopcount = 0);
	virtual ~BasicConnection();

	Type		type() const;
	TcpSocket	*socket() const;
	unsigned	hopcount() const;
	void		writeLine(const std::string &content);

  protected:
	Type		_type;
	TcpSocket	*_socket;
	unsigned	_hopcount;
};
