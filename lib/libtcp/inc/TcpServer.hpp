#pragma once
#include <list>
#include <netdb.h>
#include "Listener.hpp"
#include "TcpSocket.hpp"
#include "SslContext.hpp"

namespace tcp
{

class TcpServer
{
  public:
	TcpServer();
	~TcpServer() throw();

	size_t				nbConnections() const;
	const std::string	&host() const;
	void				loadSslConfig(const std::string &certificatePath, const std::string &keyPath);
	void				listen(const std::string &port, bool tls = false, size_t maxQueueLen = 5);
	void				setMaxConnections(size_t MaxConnections);

	void				disconnect(TcpSocket *client) throw();
	TcpSocket			*nextNewConnection() throw();
	TcpSocket			*nextPendingConnection() throw();
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
	std::string				_host;
	SslContext				_ctx;

	void setUpListener(addrinfo *a, Listener *listener);

	TcpServer(const TcpServer &);
	TcpServer &operator=(const TcpServer &);
};

} /* end of namespace tcp */
