#pragma once
#include <sys/socket.h>

namespace tcp
{

/**
 * A tcp sockstream
 */
class Socket
{
  public:
	Socket();
	virtual ~Socket() throw();

	/**
	 * \return Fd of the socket (-1 if socket() method was not call)
	 */
	int						fd() const;

	/**
	 * \return Adress bound to the fd
	 */
	const sockaddr_storage	&addr() const;

	/**
	 * \return Family of the socket
	 */
	sa_family_t				family() const;

	/**
	 * socket() system call
	 * \param family Family of the desired socket (v4 or v6)
	 */
	void					socket(int family);

	/**
	 * close() system call
	 */
	int						close() throw();

	/**
	 * bind() system call
	 */
	void					bind(sockaddr *addr, socklen_t addr_size);

	/**
	 * setsockopt() system call with SO_REUSEADDR option
	 */
	void					setReuseAddr();

	/**
	 * setsockopt() system call with IPV6_V6ONLY option
	 */
	void					setIpv6only();

	/**
	 * fcntl() system call with O_NONBLOCK option
	 */
	void					setNonblock();

	/**
	 * Voids the effects of setNonblock() method
	 */
	void					unsetNonblock();

  protected:
	int					_fd;
	sockaddr_storage	_addr;

  private:
	Socket(const Socket &other);
	Socket &operator=(const Socket &other);
};

} /* end of namespace tcp */
