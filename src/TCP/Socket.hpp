#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include "exceptions.hpp"

/**
 * \class Socket
 * \brief A generic two-way connection-based socket both compatible with IPv4 and IPv6 network layer
 * \brief so that inherited objects can perform basics sockets operations
 */
class Socket
{
  public:
	Socket();
	virtual ~Socket() throw();

	int						fd() const;
	const sockaddr_storage	&addr() const;
	sa_family_t				family() const;

	void					socket(int family);
	int						close() throw();
	void					bind(sockaddr *addr, socklen_t addr_size);
	void					setReuseAddr();
	void					setIpv6only();
	void					setNonblock();

  protected:
	int					_fd;
	sockaddr_storage	_addr;

  private:
	Socket(const Socket &other);
	Socket &operator=(const Socket &other);
};
