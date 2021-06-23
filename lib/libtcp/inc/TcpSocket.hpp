#pragma once
#include <string>
#include "Socket.hpp"
#include "stdint.h"

namespace tcp
{

/**
 * An accepted/connected Socket associated to a buffer
 */
class TcpSocket : public Socket
{
  public:
	virtual ~TcpSocket() throw();

	/**
	 * \return Human readable ip address
	 */
	const std::string	&ip() const;

	/**
	 * \return Human readable port
	 */
	uint16_t			port() const;

	/**
	 * \return Domain name associated to the ip if it exists
	 */
	const std::string	&host() const;

	/**
	 * \return Size of the already received data
	 */
	size_t				readBufSize() const;

	/**
	 * \return if there is a received line in the input buffer
	 */
	bool				isLine();

	/**
	 * \return if a call to readLine does something
	 */
	bool				canReadLine();

	/**
	 * First tries to get a line from the input buffer
	 * If there is none calls fill() and tries again
	 * \param line String into which the line is put
	 * \return false if connection is closed (fill() returned 0)
	 */
	bool				readLine(std::string &line);

	/**
	 * Apends a string to the output buffer with a newline
	 */
	void				writeLine(const std::string &data) throw();

	/**
	 * If the socket is readable, recv() 1024 char and puts them in the input buffer
	 * A TcpSocket is readable if TcpServer::select() is called beforehand or if it is inside a TcpClient
	 * \return false if connection is closed (recv() returned 0)
	 */
	virtual bool		fill();

	/**
	 * If the socket is writeable, send() the output buffer
	 * A TcpSocket is readable if TcpServer::select() is called beforehand or if it is inside a TcpClient
	 */
	virtual void		flush();

  protected:

	TcpSocket();
	TcpSocket(int listenerFd);

	static const size_t ipv6MaxSize;
	static const size_t readSize;

	std::string	_ip;
	uint16_t	_port;
	std::string	_host;
	bool		_isReadable;
	bool		_isWriteable;
	std::string	_readBuf;
	std::string	_writeBuf;
	size_t		_newline;

	virtual int		recv(void *buf, size_t size, int flags = 0);
	virtual int		send(const void *buf, size_t size, int flags = 0);
	virtual bool	isWbufEmpty() const;

  private:
	TcpSocket(const TcpSocket &other);
	TcpSocket &operator=(const TcpSocket &other);

	friend class TcpServer;
	friend class TcpClient;
	friend class Listener;
};

} /* end of namespace tcp */
