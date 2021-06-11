#include "SslContext.hpp"

bool sslInit = false;

SslContext::SslContext(const char *certificatePath, const char *keyPath)
{
	if (!sslInit)
	{
		SSL_library_init();
		OpenSSL_add_all_algorithms();
		SSL_load_error_strings();
		sslInit = true;
	}
	if (!(_ctx = SSL_CTX_new(SSLv23_server_method())))
		throw SslException("SSL_CTX_new");
  try {
	if (SSL_CTX_use_certificate_file(_ctx, certificatePath, SSL_FILETYPE_PEM) <= 0)
		throw SslException("SSL_CTX_use_certificate_file");
	if (SSL_CTX_use_PrivateKey_file(_ctx, keyPath, SSL_FILETYPE_PEM ) <= 0)
		throw SslException("SSL_CTX_use_PrivateKey_file");
	if (!SSL_CTX_check_private_key(_ctx))
		throw SslException("SSL_CTX_check_private_key");
  } catch (std::exception &e) {
		SSL_CTX_free(_ctx);
		throw ;
  }
}

SslContext::~SslContext() throw()
{
	SSL_CTX_free(_ctx);
}

SSL_CTX *SslContext::ctx() { return _ctx; }
