#pragma once
#include <list>
#include <netdb.h>
#include "Listener.hpp"
#include "TcpSocket.hpp"
#include "SslContext.hpp"

namespace tcp
{

/**
 * A server using tcp which can be herited
 */
class TcpServer
{
  public:
	TcpServer();
	virtual ~TcpServer() throw();

	/**
	 * \return The number of current connections to server
	 */
	size_t				nbConnections() const;

	/**
	 * Calls SslContext::load()
	 * \param certificatePath Path toward the ssl certificate file
	 * \param keyPath Path toward the sll key file
	 */
	void				loadSslConfig(const std::string &certificatePath, const std::string &keyPath);

	/**
	 * Creates a Listener listening to the port
	 * \param port The listened port
	 * \param tls if true creates a SslListener instead
	 * \param maxQueueLen Maximum number of simultanate connections
	 */
	void				listen(const std::string &port, bool tls = false, size_t maxQueueLen = 5);

	/**
	 * Sets maximum number of active connections
	 * \param MaxConnections The maximum number of active connections
	 */
	void				setMaxConnections(size_t MaxConnections);

	/**
	 *  Deletes a TcpSocket
	 */
	void				disconnect(TcpSocket *client) throw();

	/**
	 * Pops the first new connection
	 */
	TcpSocket			*nextNewConnection() throw();

	/**
	 * Pops the first pending connection
	 */
	TcpSocket			*nextPendingConnection() throw();

	/**
	 * Blocks, returns if one of the connections can read or write
	 */
	void				select();

	class SigintException : public std::exception {};

  private:

	static const size_t defaultMaxConnections;
	static const timeval selectTimeout;

	addrinfo				_hint;
	std::list<Listener*>	_listeners;
	std::list<Listener*>	_incoming;
	std::list<TcpSocket*>	_connections;
	std::list<TcpSocket*>	_pending;
	size_t					_maxConnections;
	SslContext				_ctx;

	TcpServer(const TcpServer &);
	TcpServer &operator=(const TcpServer &);
};

} /* end of namespace tcp */
