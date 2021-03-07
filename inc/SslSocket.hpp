/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SslSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 13:11:14 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/07 13:34:30 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <openssl/ssl.h>
#include "TcpSocket.hpp"

class SslSocket : public TcpSocket
{
public:
	enum State { FAILURE, SUCCESS, DOIT_FAIRE_SES_PREUVES };

	SslSocket(int fd, SSL_CTX *ctx);
	virtual ~SslSocket();

	std::string			readLine();
	SslSocket::State	state();

protected:
	void flush();

private:
	SSL		*_session;
	State	_state;

	SslSocket(const SslSocket &other);
	SslSocket &operator=(const SslSocket &other);
};
