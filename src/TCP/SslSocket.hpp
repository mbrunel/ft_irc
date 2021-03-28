/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SslSocket.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 13:11:14 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/15 14:50:54 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <openssl/ssl.h>
#include <errno.h>
#include <string.h>
#include "exceptions.hpp"
#include "TcpSocket.hpp"
#include "SslContext.hpp"

class SslSocket : public TcpSocket
{
  public:
	enum State {SUCCESS = 1, WANT_READ = SSL_ERROR_WANT_READ, WANT_WRITE = SSL_ERROR_WANT_WRITE};

	SslSocket(int listenerFd, SSL_CTX *ctx);
	virtual ~SslSocket() throw();

	int					IO();
	SslSocket::State	state() const;

  protected:
	int		recv(void *buf, size_t n, int flags = 0);
	int		send(const void *buf, size_t n, int flags = 0);
	bool	isWbufEmpty() const;

  private:
	SSL		*_session;
	State	_state;

	void SSL_accept();

	SslSocket(const SslSocket &other);
	SslSocket &operator=(const SslSocket &other);
};
