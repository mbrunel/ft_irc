/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SslListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 13:08:46 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/15 00:50:46 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <errno.h>
#include <openssl/ssl.h>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <string.h>
#include "Listener.hpp"
#include "TcpSocket.hpp"
#include "SslSocket.hpp"

class SslListener : public Listener
{
  public:
	SslListener(int family, SSL_CTX *ctx);
	virtual ~SslListener() throw();

	TcpSocket	*accept();

  private:
	SSL_CTX *_ctx;

	SslListener(const SslListener &);
	SslListener &operator=(const SslListener &);
};
