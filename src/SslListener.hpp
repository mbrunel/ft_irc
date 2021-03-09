/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SslListener.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 13:08:46 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/08 13:39:17 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <openssl/ssl.h>
#include <stdint.h>
#include <string>
#include "Listener.hpp"
#include "TcpSocket.hpp"

/**
 * \struct SslConfig
 * 
 */
struct SslConfig
{
	std::string certificatePath;
	std::string certificateKeyPath;

	SslConfig(const std::string &certificatePath, const std::string &certificateKeyPath);
};

class SslListener : public Listener
{
  public:
	SslListener(const SslConfig &config, uint16_t port, size_t maxConnections = 1024);
	virtual ~SslListener();

	TcpSocket*	accept();

  private:
	SSL_CTX	*_ctx;

	SslListener(const SslListener &);
	SslListener &operator=(const SslListener &);
};
