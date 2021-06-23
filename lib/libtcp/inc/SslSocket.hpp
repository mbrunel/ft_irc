#pragma once
#include <openssl/ssl.h>
#include "TcpSocket.hpp"

namespace tcp
{

/**
 * A TcpSocket which uses tls protocol
 */
class SslSocket : public TcpSocket
{
  public:
	enum State {SUCCESS = 1, WANT_READ = SSL_ERROR_WANT_READ, WANT_WRITE = SSL_ERROR_WANT_WRITE};

	virtual ~SslSocket() throw();

	/**
	 * Performs tls handshake or call TcpSocket::fill()
	 * \return false if connection is close (SSL_read return 0)
	 */
	bool				fill();

	/**
	 * Performs tls handshake or call TcpSocket::flush()
	 */
	void				flush();

	/**
	 * \return State of completion of tls handshake
	 */
	SslSocket::State	state() const;

  protected:

	SslSocket(SSL_CTX *ctx);
	SslSocket(int listenerFd, SSL_CTX *ctx);
	void	socket(int family);
	int		recv(void *buf, size_t n, int flags = 0);
	int		send(const void *buf, size_t n, int flags = 0);
	bool	isWbufEmpty() const;

  private:
	SSL			*_session;
	State		_state;

	void SSL_accept();
	void SSL_connect();

	SslSocket(const SslSocket &other);
	SslSocket &operator=(const SslSocket &other);

	friend class SslListener;
	friend class TcpClient;
};

} /* end of namespace tcp */
