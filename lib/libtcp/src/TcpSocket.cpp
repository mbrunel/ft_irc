#include "TcpSocket.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "libft.hpp"

namespace tcp
{

const size_t TcpSocket::ipv6MaxSize = 39;
const size_t TcpSocket::readSize = 1024;

TcpSocket::TcpSocket():Socket(), _isReadable(false), _isWriteable(false), _newline(std::string::npos) {}

TcpSocket::TcpSocket(int listenerFd):Socket(), _isReadable(false), _isWriteable(false), _newline(std::string::npos)
{
	socklen_t addrLen = sizeof(_addr);
	if ((_fd = accept(listenerFd, (sockaddr *)&_addr, &addrLen)) == -1)
		throw ft::systemError("accept");
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
			throw ft::systemError("inet_ntop");
		}
		_ip = buf;
	}
	else
	{
		close();
		throw ft::systemError(EPFNOSUPPORT, "accept");
	}
	char buf[NI_MAXHOST];
	if (!getnameinfo((sockaddr *)&_addr, addrLen, buf, sizeof(buf), NULL, 0, NI_NAMEREQD))
		_host = buf;
	else
		_host = _ip;
}


TcpSocket::~TcpSocket() throw() { close(); }

bool TcpSocket::isWbufEmpty() const { return (_writeBuf.empty()); }

size_t TcpSocket::readBufSize() const { return (_readBuf.size()); }

void TcpSocket::writeLine(const std::string &data) throw()
{
	_writeBuf += data + '\n';
}

bool TcpSocket::isLine()
{
	if (_newline == std::string::npos)
		if ((_newline = _readBuf.find_first_of('\n')) == std::string::npos)
			if ((_newline = _readBuf.find_first_of('\r')) == std::string::npos)
				return false;
	return true;
}

bool TcpSocket::canReadLine()
{
	return (isLine() || _isReadable);
}

bool TcpSocket::readLine(std::string &line)
{
	line.clear();
	if (!isLine())
		if (!fill())
			return false;
	line = _readBuf.substr(0, _newline + 1);
	_readBuf.erase(0, _newline + 1);
	_newline = std::string::npos;
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
		buf[nb > 0 ? nb : 0] = '\0';
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
		if (errno != EWOULDBLOCK)
			throw ft::systemError("send");
	return (nb);
}

int TcpSocket::recv(void *buf, size_t n, int flags)
{
	int nb;
	if ((nb = ::recv(_fd, buf, n, flags)) == -1)
		if (errno != EWOULDBLOCK)
			throw ft::systemError("recv");
	return (nb);
}

const std::string &TcpSocket::ip() const { return (_ip); }

uint16_t TcpSocket::port() const { return (_port); }

const std::string &TcpSocket::host() const { return _host; }

} /* end of namespace tcp */
