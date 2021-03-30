/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 12:56:19 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/30 02:55:41 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include "Socket.hpp"

class TcpSocket : public Socket
{
  public:
	TcpSocket(int listenerFd);
	virtual ~TcpSocket() throw();

	const std::string	&ip() const;
	uint16_t			port() const;
	const std::string	&host() const;

	void				writeLine(const std::string &data) throw();
	bool				readLine(std::string &line) throw();
	virtual int			IO();

  protected:
	std::string	_ip;
	uint16_t	_port;
	std::string	_host;
	bool		_isReadable;
	bool		_isWriteable;
	std::string	_readBuf;
	std::string	_writeBuf;

	virtual int recv(void *buf, size_t size, int flags = 0);
	virtual int send(const void *buf, size_t size, int flags = 0);
	virtual bool isRbufEmpty() const;
	virtual bool isWbufEmpty() const;

  private:
	TcpSocket();
	TcpSocket(const TcpSocket &other);
	TcpSocket &operator=(const TcpSocket &other);

	friend class TcpServer;
};
