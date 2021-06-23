#pragma once
#include "Listener.hpp"
#include "TcpSocket.hpp"
#include <openssl/ssl.h>

namespace tcp
{

/**
 * A Listener which creates SslSockets instead
 */
class SslListener : public Listener
{
  public:

	/**
	 * \param family Family of the address (v4 or v6)
	 * \param ctx Ssl context contained in SslContext
	 */
	SslListener(int family, SSL_CTX *ctx);
	virtual ~SslListener() throw();

	/**
	 * Creates a new SslSocket
	 */
	TcpSocket	*accept();

  private:
	SSL_CTX *_ctx;

	SslListener(const SslListener &);
	SslListener &operator=(const SslListener &);
};

} /* end of namespace tcp */
