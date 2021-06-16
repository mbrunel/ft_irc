#include "TcpSocket.hpp"

TcpSocket::TcpSocket(int listenerFd):Socket(), _isReadable(false), _isWriteable(false)
{
	socklen_t addrLen = sizeof(_addr);
	if ((_fd = accept(listenerFd, (sockaddr *)&_addr, &addrLen)) == -1)
		throw ErrnoException("accept");
	if (family() == AF_INET)
	{
		_port = ntohs(((sockaddr_in *)&_addr)->sin_port);
		_ip = inet_ntoa(((sockaddr_in *)&_addr)->sin_addr);
	}
	else if (family() == AF_INET6)
	{
		char buf[100];
		_port = ntohs(((sockaddr_in6 *)&_addr)->sin6_port);
		if (!(inet_ntop(AF_INET6, &_addr, buf, 100)))
		{
			close();
			throw ErrnoException("inet_ntop");
		}
		_ip = buf;
	}
	else
	{
		close();
		throw MsgException("Unknown IP family");
	}
	char buf[NI_MAXHOST];
	if (!getnameinfo((sockaddr *)&_addr, addrLen, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		_host = buf;
	else
		_host = _ip;
}

TcpSocket::~TcpSocket() throw() { close(); }

bool TcpSocket::isRbufEmpty() const { return (_readBuf.empty()); }

bool TcpSocket::isWbufEmpty() const { return (_writeBuf.empty()); }

void TcpSocket::writeLine(const std::string &data) throw()
{
	_writeBuf += data + '\n';
}

bool TcpSocket::readLine(std::string &line) throw()
{
	line = "";
	size_t sep;
	if ((sep = _readBuf.find_first_of('\n')) == std::string::npos)
		if ((sep = _readBuf.find_first_of('\r')) == std::string::npos)
			return false;
	line = _readBuf.substr(0, sep + 1);
	_readBuf.erase(0, sep + 1);
	return (true);
}

int TcpSocket::IO()
{
	if (_isWriteable && !isWbufEmpty())
	{
		_isWriteable = false;
		send(_writeBuf.c_str(), _writeBuf.size());
		_writeBuf.clear();
	}
	if (_isReadable)
	{
		char buf[1025];
		int nb;
		_isReadable = false;
		if (!(nb = recv(buf, 1024)))
			return (0);
		buf[nb] = '\0';
		_readBuf += buf;
		if (_readBuf.size() >= 2048)
			throw MsgException("socket is flooded");
	}
	return (1);
}

int TcpSocket::send(const void *buf, size_t n, int flags)
{
	int nb;
	if ((nb = ::send(_fd, buf, n, flags)) == -1)
		throw ErrnoException("send");
	return (nb);
}

int TcpSocket::recv(void *buf, size_t n, int flags)
{
	int nb;
	if ((nb = ::recv(_fd, buf, n, flags)) == -1)
		throw ErrnoException("recv");
	return (nb);
}

const std::string &TcpSocket::ip() const { return (_ip); }

uint16_t TcpSocket::port() const { return (_port); }

const std::string &TcpSocket::host() const { return _host; }
