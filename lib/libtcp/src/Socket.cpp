#include "Socket.hpp"
#include <cstring>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "system_error.hpp"
#include <unistd.h>

namespace tcp
{

Socket::Socket():_fd(-1) {}

Socket::~Socket() throw() {}

int Socket::fd() const { return (_fd); }

const sockaddr_storage &Socket::addr() const { return (_addr); }

sa_family_t Socket::family() const { return (_addr.ss_family); }

void Socket::socket(int family)
{
	if ((_fd = ::socket(family, SOCK_STREAM, IPPROTO_TCP)) == -1)
		throw ft::system_error("socket");
}

int Socket::close() throw()
{
	int s = 0;
	if (_fd != -1)
		s = ::close(_fd);
	_fd = -1;
	return s;
}

void Socket::bind(sockaddr *addr, socklen_t addr_size)
{
	if (::bind(_fd, addr, addr_size) == -1)
		throw ft::system_error("bind");
	std::memcpy(&_addr, &addr, addr_size);
}

void Socket::setReuseAddr()
{
	int reuse = 1;
	if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
		throw ft::system_error("setsockopt");
}

void Socket::setIpv6only()
{
	int v6Only = 1;
	if (::setsockopt(_fd, IPPROTO_IPV6, IPV6_V6ONLY, &v6Only, sizeof(v6Only)) == -1)
		throw ft::system_error("setsockopt");
}

void Socket::setNonblock()
{
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
		throw ft::system_error("fcntl");
}

void Socket::unsetNonblock()
{
	int oldfl;
	if ((oldfl = fcntl(_fd, F_GETFL)) == -1)
		throw ft::system_error("fcntl");
	fcntl(_fd, F_SETFL, oldfl & ~O_NONBLOCK);
}

} /* end of namespace tcp */
