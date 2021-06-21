#include "TcpServer.hpp"
#include "SslListener.hpp"
#include "libft.hpp"
#include <sys/select.h>
#include <sys/signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

namespace tcp
{

bool alive = true;

const size_t TcpServer::defaultMaxConnections = 1024;
const timeval TcpServer::selectTimeout = {5, 0};

void abortSelect(int sig) throw()
{
	(void)sig;
	alive = false;
}

TcpServer::TcpServer():_maxConnections(1024), _ctx(SslContext::SERVER)
{
	bzero(&_hint, sizeof(_hint));
	_hint.ai_family = AF_UNSPEC;
	_hint.ai_socktype = SOCK_STREAM;
	_hint.ai_protocol = IPPROTO_TCP;
	_hint.ai_flags = AI_PASSIVE;
	signal(SIGINT, abortSelect);
	signal(SIGPIPE, SIG_IGN);
	siginterrupt(SIGINT, 1);
}

TcpServer::~TcpServer() throw()
{
	while (!_connections.empty())
		disconnect(_connections.front());
	for (std::list<Listener *>::iterator it = _listeners.begin(); it != _listeners.end(); it++)
		delete *it;
}

void TcpServer::setUpListener(addrinfo *a, Listener *listener)
{
	listener->setReuseAddr();
	listener->setNonblock();
	if (a->ai_family == AF_INET6)
		listener->setIpv6only();
	listener->bind(a->ai_addr, a->ai_addrlen);
}

size_t TcpServer::nbConnections() const { return (_connections.size()); }

const std::string &TcpServer::host() const { return (_host); }

void TcpServer::setMaxConnections(size_t maxConnections) { _maxConnections = maxConnections; }

void TcpServer::loadSslConfig(const std::string &certificatePath, const std::string &keyPath)
{
	_ctx.load(certificatePath.c_str(), keyPath.c_str());
}

void TcpServer::listen(const std::string &port, bool tls, size_t maxQueueLen)
{
	addrinfo *head, *node;
	int i = 0;

	if (getaddrinfo(NULL, port.c_str(), &_hint, &head))
		throw ft::systemError("getaddrinfo");
	node = head;
	try {
		while (node && i < 2)
		{
			ft::unique_ptr<Listener> newListener;
			if (!tls)
				newListener.reset(new Listener(node->ai_family));
			else
				newListener.reset(new SslListener(node->ai_family, _ctx.ctx()));
			setUpListener(node, newListener.get());
			newListener->listen(maxQueueLen);
			_listeners.push_back(newListener.get());
			node = node->ai_next;
			i++;
			newListener.release();
		}
	}
	catch (std::exception &e) {
		freeaddrinfo(head);
		throw;
	}
	freeaddrinfo(head);
}

void TcpServer::disconnect(TcpSocket *connection) throw()
{
	_pending.remove(connection);
	_connections.remove(connection);
	delete connection;
}

TcpSocket *TcpServer::nextNewConnection() throw()
{
	if (_incoming.empty())
		return (NULL);
	Listener *listener = _incoming.front();
	_incoming.pop_front();
	ft::unique_ptr<TcpSocket> newConnection;
	try {
		newConnection.reset(listener->accept());
		if (_connections.size() == _maxConnections)
			throw std::exception();
		_connections.push_back(newConnection.get());
	}
	catch (std::exception &e) {
		return nextNewConnection();
	}
	return newConnection.release();
}

TcpSocket *TcpServer::nextPendingConnection() throw()
{
	if (_pending.empty())
		return (NULL);
	TcpSocket *next = _pending.front();
	_pending.pop_front();
	return (next);
}

void TcpServer::select()
{
	fd_set readSet;
	fd_set writeSet;
	struct timeval timeout = selectTimeout;

	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	for (std::list<Listener *>::iterator it = _listeners.begin(); it != _listeners.end(); it++)
		FD_SET((*it)->fd(), &readSet);
	for (std::list<TcpSocket *>::iterator it = _connections.begin(); it != _connections.end(); it++)
	{
		TcpSocket *c = *it;
		FD_SET(c->fd(), &readSet);
		if (!c->isWbufEmpty())
			FD_SET(c->fd(), &writeSet);
	}
	if (alive)
		::select(FD_SETSIZE + 1, &readSet, &writeSet, NULL, &timeout);
	if (!alive)
		throw SigintException();
	for (std::list<Listener *>::iterator it = _listeners.begin(); it != _listeners.end(); it++)
		if (FD_ISSET((*it)->fd(), &readSet))
			_incoming.push_back(*it);
	for (std::list<TcpSocket *>::iterator it = _connections.begin(); it != _connections.end(); it++)
	{
		TcpSocket *c = *it;
		if (FD_ISSET(c->fd(), &readSet))
			c->_isReadable = true;
		if (FD_ISSET(c->fd(), &writeSet))
			c->_isWriteable = true;
		if (c->canReadLine() || c->_isWriteable)
			_pending.push_back(c);
	}
}

} /* end of namespace tcp */
