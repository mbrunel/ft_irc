#pragma once
#include <sys/select.h>
#include <sys/signal.h>
#include <iostream>
#include <fstream>
#include <list>
#include <time.h>
#include <assert.h>
#include "Listener.hpp"
#include "TcpSocket.hpp"
#include "SslListener.hpp"
#include "SslContext.hpp"
#include "Config.hpp"

class TcpServer
{
  public:
	TcpServer();
	~TcpServer() throw();

	void				init(Config &cfg);
	size_t				nbConnections() const;
	const std::string	&host() const;
	void				listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);
	void				setMaxConnections(size_t MaxConnections);
	void				setVerbose(bool verbose);
	void				setLogDestination(const std::string &destfile);

	void				disconnect(TcpSocket *client) throw();
	std::ostream		&log() throw();
	TcpSocket			*nextNewConnection() throw();
	TcpSocket			*nextPendingConnection() throw();
	void				select();

	class SigintException : public std::exception {};

  private:
	addrinfo				_hint;
	std::list<Listener*>	_listeners;
	std::list<Listener*>	_incoming;
	std::list<TcpSocket*>	_connections;
	std::list<TcpSocket*>	_pending;
	size_t					_maxConnections;
	bool					_verbose;
	std::ostream			_log;
	std::ofstream			_logfile;
	std::string				_host;
	SslContext				_ctx;

	void setUpListener(addrinfo *a, Listener *listener);

	TcpServer(const TcpServer &);
	TcpServer &operator=(const TcpServer &);
};
