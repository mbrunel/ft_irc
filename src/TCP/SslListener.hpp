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
