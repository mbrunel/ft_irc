#include "SslContext.hpp"

namespace tcp
{

bool _sslInit = false;

SslContext::SslContext()
{
	if (!_sslInit)
	{
		SSL_library_init();
		OpenSSL_add_all_algorithms();
		SSL_load_error_strings();
		ERR_load_crypto_strings();
		_sslInit = true;
	}
	if (!(_ctx = SSL_CTX_new(SSLv23_server_method())))
		throw tcp::SslException("SSL_CTX_new");
}

void SslContext::load(const char *certificatePath, const char *keyPath)
{
	if (SSL_CTX_use_certificate_file(_ctx, certificatePath, SSL_FILETYPE_PEM) != 1)
		throw tcp::SslException("SSL_CTX_use_certificate_file");
	if (SSL_CTX_use_PrivateKey_file(_ctx, keyPath, SSL_FILETYPE_PEM ) != 1)
		throw tcp::SslException("SSL_CTX_use_PrivateKey_file");
	if (SSL_CTX_check_private_key(_ctx) != 1)
		throw tcp::SslException("SSL_CTX_check_private_key");
}

SslContext::~SslContext() throw()
{
	SSL_CTX_free(_ctx);
}

SSL_CTX *SslContext::ctx() { return _ctx; }

} /* end of namespace tcp */
