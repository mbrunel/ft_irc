/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 12:56:19 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/07 13:09:26 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "Socket.hpp"

class TcpSocket : public Socket
{
public:
	TcpSocket(int fd);
	virtual ~TcpSocket();

	const std::string	&hostname();
	const std::string	&ip();
	uint16_t			port();
	void				write(const std::string &data);
	virtual std::string	readLine();

protected:
	virtual void flush();

private:
	std::string	_hostname;
	std::string	_ip;
	bool		_isReadable;
	std::string	_readBuf;
	std::string	_writeBuf;

	TcpSocket(const TcpSocket &other);
	TcpSocket &operator=(const TcpSocket &other);
};
