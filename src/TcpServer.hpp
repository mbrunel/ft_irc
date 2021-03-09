/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 12:49:07 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/07 14:57:52 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <list>
#include "Listener.hpp"
#include "TcpSocket.hpp"

class TcpServer
{
  public:
	TcpServer();
	~TcpServer();

	void		disconnect(TcpSocket *client);
	void		listen(Listener *listener);
	TcpSocket	*nextIncomingConnection();
	TcpSocket	*nextPendingConnection();
	void		sendData();
	void		waitData();

  private:
	std::list<TcpSocket*>	_clients;
	std::list<Listener*>	_listeners;
	std::list<TcpSocket*>	_incoming;
	std::list<TcpSocket*>	_readable;
	std::list<TcpSocket*>	_writable;

	TcpServer(const TcpServer &);
	TcpServer &operator=(const TcpServer &);
};
