#pragma once
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <stdexcept>
#include <string.h>
#include <errno.h>

class MsgException : public std::exception
{
  public:
	MsgException(const std::string &msg):msg(msg) {}
	virtual ~MsgException() throw() {}

	const char *what() const throw() { return (msg.c_str()); }

  protected:
	std::string msg;
};

class ErrnoException : public MsgException
{
  public:
	ErrnoException(const std::string &msg = ""):MsgException(msg + ": " + strerror(errno)), _err(errno) {}
	virtual ~ErrnoException() throw() {}

	int err() const throw() { return (_err); }

  private:
	int _err;
};

class SslException : public MsgException
{
  public:
	SslException(const std::string &msg = ""):
		MsgException(msg + ": " + ERR_error_string(ERR_peek_error(), NULL)),
		_ssl_err(ERR_peek_error()) {}
	virtual ~SslException() throw() {}

	int ssl_err() const throw() { return (_ssl_err); }

  private:
	unsigned long _ssl_err;
};
