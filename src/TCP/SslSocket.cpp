#include "SslSocket.hpp"

SslSocket::SslSocket(int listenerFd, SSL_CTX *ctx):TcpSocket(listenerFd)
{
	if (!(_session = SSL_new(ctx)))
	{
		close();
		throw tcp::SslException("SSL_new");
	}
	if (SSL_set_fd(_session, _fd) == -1)
	{
		SSL_free(_session);
		close();
		throw tcp::SslException("SSL_set_fd");
	}
	_state = WANT_READ;
}

SslSocket::~SslSocket() throw()
{
	SSL_free(_session);
}

bool SslSocket::isWbufEmpty() const { return (TcpSocket::isWbufEmpty() && _state == SUCCESS); }

int SslSocket::IO()
{
	if (_state == SUCCESS)
		return TcpSocket::IO();
	if (_state == WANT_READ && _isReadable)
	{
		_isReadable = false;
		SSL_accept();
	}
	if (_state == WANT_WRITE && _isWriteable)
	{
		_isWriteable = false;
		SSL_accept();
	}
	return (1);
}

void SslSocket::SSL_accept()
{
	int ret;
	if (!(ret = ::SSL_accept(_session)))
		_state = (State)SSL_get_error(_session, 0);
	else if (ret < 0)
		throw tcp::SslException("SSL_accept");
	else
		_state = SUCCESS;
}

int SslSocket::send(const void *buf, size_t n, int flags)
{
	(void)flags;
	int nb;
	if ((nb = SSL_write(_session, buf, n)) <= 0)
		throw tcp::SslException("SSL_write");
	return (nb);
}

int SslSocket::recv(void *buf, size_t n, int flags)
{
	(void)flags;
	int nb;
	if ((nb = SSL_read(_session, buf, n)) < 0)
		throw tcp::SslException("SSL_read");
	return (nb);
}
