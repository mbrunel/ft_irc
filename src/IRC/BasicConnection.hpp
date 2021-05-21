/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BasicConnection.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 18:50:36 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/21 17:00:42 by mbrunel          ###   ########.fr       */
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
	time_t			&clock();
	bool					&pongExpected();

  protected:
	TcpSocket	*_socket;
	Type		_type;
	unsigned	_hopcount;
	time_t		_clock;
	bool		_pongExpected;

	BasicConnection(TcpSocket *socket, Type type, unsigned hopcount = 0);
};
