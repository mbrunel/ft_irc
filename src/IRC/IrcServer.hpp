/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:57 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/22 13:27:37 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "NetworkModule.hpp"
#include <map>
#include <set>

class IrcServer
{
  public:
	IrcServer();
	~IrcServer();

	void		setMaxConnections(size_t setMaxConnections);
	void		setVerbose(bool verbose);
	void		listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);

	void		log(const std::string &s, bool err = false) const throw();
	void		run() throw();

  private:
	TcpServer srv;

	void nickCmd();

	void		interact(TcpSocket *connection);
	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
