/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 01:29:06 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/28 20:20:40 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TcpServer.hpp"

int abortFd = -1;

TcpServer::TcpServer():_maxConnections(1024), _verbose(true), _log(std::cerr.rdbuf())
{
	_hint.ai_family = AF_UNSPEC;
	_hint.ai_socktype = SOCK_STREAM;
	_hint.ai_protocol = IPPROTO_TCP;
	_hint.ai_flags = AI_PASSIVE;
	signal(SIGINT, abortSelect);
}

TcpServer::~TcpServer() throw()
{
	while (!_connections.empty())
		disconnect(_connections.front());
	for (std::list<Listener *>::iterator it = _listeners.begin(); it != _listeners.end(); it++)
		delete *it;
	if (_logfile.is_open())
		_logfile.close();
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

void TcpServer::setVerbose(bool verbose) { _verbose = verbose; }

std::string timestamp() throw()
{
	char stamp[80];

	time_t rawtime;
	time(&rawtime);
	strftime(stamp, 80, "%x - %I:%M:%S", localtime(&rawtime));
	return (stamp);
}

void TcpServer::setLogDestination(const std::string &destfile)
{
	_logfile.open(destfile.c_str(), std::ios::out);
	_log.rdbuf(_logfile.rdbuf());
}

std::ostream &TcpServer::log() throw()
{
	return (_log << timestamp() << " ");
}

void TcpServer::listen(const char *port, SSL_CTX *ctx, size_t maxQueueLen)
{
	addrinfo *head, *node;
	int i = 0;

	if (getaddrinfo(NULL, port, &_hint, &head))
		throw ErrnoException("getaddrinfo");
	node = head;
	while (node && i < 2)
	{
		unique_ptr<Listener> newListener;
		if (!ctx)
			 newListener.reset(new Listener(node->ai_family));
		else
			newListener.reset(new SslListener(node->ai_family, ctx));
		setUpListener(node, newListener.get());
		newListener->listen(maxQueueLen);
		_listeners.push_back(newListener.get());
		node = node->ai_next;
		i++;
		newListener.release();
	}
	freeaddrinfo(head);
}

void TcpServer::disconnect(TcpSocket *connection) throw()
{
	log() << connection->ip() << " DISCONNECTED" << std::endl;
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
	unique_ptr<TcpSocket> newConnection;
	try {
		newConnection.reset(listener->accept());
		if (_connections.size() == _maxConnections)
			throw MsgException("Too many connections");
		_connections.push_back(newConnection.get());
		log() << newConnection->ip() << " CONNECTED" << std::endl;
	}
	catch (std::exception &e) {
		log() << e.what() << std::endl;
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

void abortSelect(int sig) throw()
{
	(void)sig;
	if (abortFd >= 0)
		close(abortFd);
	abortFd = -2;
}

void TcpServer::select()
{
	fd_set readSet;
	fd_set writeSet;

	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	if (_listeners.empty())
		throw MsgException("No open port on server.");
	for (std::list<Listener *>::iterator it = _listeners.begin(); it != _listeners.end(); it++)
		FD_SET((*it)->fd(), &readSet);
	for (std::list<TcpSocket *>::iterator it = _connections.begin(); it != _connections.end(); it++)
	{
		FD_SET((*it)->fd(), &readSet);
		if (!(*it)->isWbufEmpty())
			FD_SET((*it)->fd(), &writeSet);
	}
	abortFd = _listeners.front()->fd();
	if (abortFd != -2)
		::select(FD_SETSIZE + 1, &readSet, &writeSet, NULL, NULL);
	if (abortFd == -2)
		throw SigintException();
	abortFd = -1;
	for (std::list<Listener *>::iterator it = _listeners.begin(); it != _listeners.end(); it++)
		if (FD_ISSET((*it)->fd(), &readSet))
			_incoming.push_back(*it);
	for (std::list<TcpSocket *>::iterator it = _connections.begin(); it != _connections.end(); it++)
	{
		if (FD_ISSET((*it)->fd(), &readSet))
			(*it)->_isReadable = true;
		if (FD_ISSET((*it)->fd(), &writeSet))
			(*it)->_isWriteable = true;
		if (!(*it)->isRbufEmpty() || (*it)->_isReadable || (*it)->_isWriteable)
			_pending.push_back(*it);
	}
}