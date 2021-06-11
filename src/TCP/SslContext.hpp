#pragma once

#include <string>
#include <openssl/ssl.h>
#include "exceptions.hpp"

struct SslContext
{
  public :
	SslContext(const char *certificatePath, const char *keyPath);
	~SslContext() throw();

	SSL_CTX *ctx();

  private :
	SSL_CTX *_ctx;
};
