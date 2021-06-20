#pragma once
#include <stdexcept>
#include <openssl/err.h>
#include <openssl/ssl.h>

namespace tcp
{

class SslException : public std::runtime_error
{
public:

	SslException(const std::string &what_arg) :
	std::runtime_error(what_arg + ": " + (ERR_reason_error_string(ERR_peek_error()) ? ERR_reason_error_string(ERR_peek_error()) : "No reason")),
	_sslCode(ERR_get_error())
	{}

	virtual ~SslException() throw()
	{}

	unsigned long sslCode() const throw()
	{
		return (_sslCode);
	}

private:
	unsigned long _sslCode;
};

struct SslContext
{
  public :

	enum Type {CLIENT, SERVER};

	SslContext(Type type);
	void load(const char *certificatePath, const char *keyPath);
	~SslContext() throw();

	SSL_CTX *ctx();

  private :
	SSL_CTX *_ctx;
};

} /* end of namespace tcp */
