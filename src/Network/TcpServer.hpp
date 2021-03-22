/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 12:49:07 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/20 00:10:49 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/select.h>
#include <sys/signal.h>
#include <iostream>
#include <list>
#include <time.h>
#include "Listener.hpp"
#include "TcpSocket.hpp"
#include "SslListener.hpp"
#include "SslContext.hpp"
#include "smartptrs.hpp"

class TcpServer
{
  public:
	TcpServer();
	~TcpServer() throw();

	size_t		nbConnections() const;
	void		listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);
	void		setMaxConnections(size_t MaxConnections);
	void		setVerbose(bool verbose);

	void		disconnect(TcpSocket *client) throw();
	void		log(const std::string &s, bool err = false) const throw();
	TcpSocket	*nextNewConnection() throw();
	TcpSocket	*nextPendingConnection() throw();
	void		select();

	class SigintException : public std::exception {};

  private:
	addrinfo				_hint;
	std::list<Listener*>	_listeners;
	std::list<Listener*>	_incoming;
	std::list<TcpSocket*>	_connections;
	std::list<TcpSocket*>	_pending;
	size_t					_maxConnections;
	bool					_verbose;

	void setUpListener(addrinfo *a, Listener *listener);

	TcpServer(const TcpServer &);
	TcpServer &operator=(const TcpServer &);
};

std::string	timestamp() throw();
void		abortSelect(int sig) throw();
