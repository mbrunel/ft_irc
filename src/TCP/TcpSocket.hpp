#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include "Socket.hpp"

class TcpSocket : public Socket
{
  public:
	TcpSocket(int listenerFd);
	virtual ~TcpSocket() throw();

	const std::string	&ip() const;
	uint16_t			port() const;
	const std::string	&host() const;
	size_t				readBufSize() const;

	bool				readLine(std::string &line);
	void				writeLine(const std::string &data) throw();
	virtual bool		fill();
	virtual void		flush();

  protected:

	TcpSocket(int fd, sockaddr *addr, socklen_t addrlen);

	static const size_t ipv6MaxSize;
	static const size_t readSize;

	std::string	_ip;
	uint16_t	_port;
	std::string	_host;
	bool		_isReadable;
	bool		_isWriteable;
	std::string	_readBuf;
	std::string	_writeBuf;

	virtual int		recv(void *buf, size_t size, int flags = 0);
	virtual int		send(const void *buf, size_t size, int flags = 0);
	virtual bool	isWbufEmpty() const;

  private:
	TcpSocket();
	TcpSocket(const TcpSocket &other);
	TcpSocket &operator=(const TcpSocket &other);

	friend class TcpServer;
	friend class Listener;
};
