#pragma once
#include <sys/socket.h>

namespace tcp
{

class Socket
{
  public:
	Socket();
	virtual ~Socket() throw();

	int						fd() const;
	const sockaddr_storage	&addr() const;
	sa_family_t				family() const;

	virtual void			socket(int family);
	int						close() throw();
	void					bind(sockaddr *addr, socklen_t addr_size);
	void					setReuseAddr();
	void					setIpv6only();
	void					setNonblock();
	void					unsetNonblock();

  protected:
	int					_fd;
	sockaddr_storage	_addr;

  private:
	Socket(const Socket &other);
	Socket &operator=(const Socket &other);
};

} /* end of namespace tcp */
