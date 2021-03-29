/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 18:50:36 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/29 19:11:58 by asoursou         ###   ########.fr       */
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
		USER
	};

	virtual ~BasicConnection();

	Type			type() const;
	TcpSocket		*socket() const;
	unsigned		hopcount() const;
	void			writeLine(const std::string &content);

  protected:
	TcpSocket	*_socket;
	Type		_type;
	unsigned	_hopcount;

	BasicConnection(TcpSocket *socket, Type type, unsigned hopcount = 0);
};
