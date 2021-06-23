#pragma once
#include <stdexcept>
#include <openssl/err.h>
#include <openssl/ssl.h>

namespace tcp
{

/**
 * Exception thrown to get libssl error messages
 */
class SslException : public std::runtime_error
{
public:

	/**
	 * \param what_arg Name of the failing function
	 */
	SslException(const std::string &what_arg) :
	std::runtime_error(what_arg + ": " + (ERR_reason_error_string(ERR_peek_error()) ? ERR_reason_error_string(ERR_peek_error()) : "No reason")),
	_sslCode(ERR_get_error())
	{}

	virtual ~SslException() throw()
	{}

	/**
	 * \return the libssl error code
	 */
	unsigned long sslCode() const throw()
	{
		return (_sslCode);
	}

private:
	unsigned long _sslCode;
};

/**
 * libssl's context
 */
class SslContext
{
  public :

	enum Type {CLIENT, SERVER};

	/**
	 * Inits the ssl lib
	 * \param type Indicates if ssl should use client or server protocol
	 */
	SslContext(Type type);

	/**
	 * Loads and checks the validity of the certificate and key files;
	 * \param certificatePath Path toward the ssl certificate file
	 * \param keyPath Path toward the sll key file
	 */
	void load(const char *certificatePath, const char *keyPath);
	~SslContext() throw();

	SSL_CTX *ctx();

  private :
	SSL_CTX *_ctx;
};

} /* end of namespace tcp */
