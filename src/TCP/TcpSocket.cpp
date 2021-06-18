#include "TcpSocket.hpp"
#include <iostream>
const size_t TcpSocket::ipv6MaxSize = 39;
const size_t TcpSocket::readSize = 1024;

TcpSocket::TcpSocket(int listenerFd):Socket(), _isReadable(false), _isWriteable(false)
{
	socklen_t addrLen = sizeof(_addr);
	if ((_fd = accept(listenerFd, (sockaddr *)&_addr, &addrLen)) == -1)
		throw ft::system_error("accept");
	if (family() == AF_INET)
	{
		_port = ntohs(((sockaddr_in *)&_addr)->sin_port);
		_ip = inet_ntoa(((sockaddr_in *)&_addr)->sin_addr);
	}
	else if (family() == AF_INET6)
	{
		char buf[ipv6MaxSize];
		_port = ntohs(((sockaddr_in6 *)&_addr)->sin6_port);
		if (!(inet_ntop(AF_INET6, &_addr, buf, ipv6MaxSize)))
		{
			close();
			throw ft::system_error("inet_ntop");
		}
		_ip = buf;
	}
	else
	{
		close();
		throw ft::system_error(EPFNOSUPPORT, "accept");
	}
	char buf[NI_MAXHOST];
	if (!getnameinfo((sockaddr *)&_addr, addrLen, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		_host = buf;
	else
		_host = _ip;
}


TcpSocket::~TcpSocket() throw() { close(); }

bool TcpSocket::isWbufEmpty() const { return (_writeBuf.empty()); }

size_t TcpSocket::readBufSize() const { return (_writeBuf.size()); }

void TcpSocket::writeLine(const std::string &data) throw()
{
	_writeBuf += data + '\n';
}

bool TcpSocket::readLine(std::string &line)
{
	if (!fill())
		return false;
	line.clear();
	size_t sep;
	if ((sep = _readBuf.find_first_of('\n')) == std::string::npos)
		if ((sep = _readBuf.find_first_of('\r')) == std::string::npos)
			return true;
	line = _readBuf.substr(0, sep + 1);
	_readBuf.erase(0, sep + 1);
	return (true);
}

bool TcpSocket::fill()
{
	if (_isReadable)
	{
		char buf[readSize + 1];
		int nb;
		_isReadable = false;
		if (!(nb = recv(buf, readSize)))
			return false;
		buf[nb] = '\0';
		_readBuf += buf;
	}
	return true;
}

void TcpSocket::flush()
{
	if (!_isWriteable || isWbufEmpty())
		return ;
	_isWriteable = false;
	send(_writeBuf.c_str(), _writeBuf.size());
	_writeBuf.clear();
}

int TcpSocket::send(const void *buf, size_t n, int flags)
{
	int nb;
	if ((nb = ::send(_fd, buf, n, flags)) == -1)
		throw ft::system_error("send");
	return (nb);
}

int TcpSocket::recv(void *buf, size_t n, int flags)
{
	int nb;
	if ((nb = ::recv(_fd, buf, n, flags)) == -1)
		throw ft::system_error("recv");
	return (nb);
}

const std::string &TcpSocket::ip() const { return (_ip); }

uint16_t TcpSocket::port() const { return (_port); }

const std::string &TcpSocket::host() const { return _host; }
