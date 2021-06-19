#pragma once
#include "Listener.hpp"
#include "TcpSocket.hpp"
#include <openssl/ssl.h>

namespace tcp
{

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

} /* end of namespace tcp */
