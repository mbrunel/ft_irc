/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 13:49:11 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

IrcServer::IrcServer() {}

IrcServer::~IrcServer() {}

void IrcServer::setMaxConnections(size_t MaxConnections) { srv.setMaxConnections(MaxConnections); }

void IrcServer::setVerbose(bool verbose) { srv.setVerbose(verbose); }

void IrcServer::log(const std::string &s, bool err) const throw() { srv.log(s, err); }

void IrcServer::listen(const char *port, SSL_CTX *ctx, size_t maxQueueLen) { srv.listen(port, ctx, maxQueueLen); }

void IrcServer::run() throw()
{
	while (1)
	{
		try { srv.select(); } catch(TcpServer::SigintException &e) { srv.log("SIGINT catched, exiting properly"); return ; }
		TcpSocket *newConnection;
		while ((newConnection = srv.nextNewConnection()))
			newConnection->writeLine("BONJOUR CHER CLIENT");
		TcpSocket *Connection;
		while ((Connection = srv.nextPendingConnection()))
		{
			try { if (!Connection->IO()) { srv.disconnect(Connection); continue ; } }
			catch (std::exception &e) { srv.log(e.what(), true); srv.disconnect(Connection); continue ; }
			std::string line;
			if (!Connection->readLine(line))
				continue ;
			srv.log(line);
			Message msg(line);
			srv.log(line);
			std::cout << msg.isValid() << std::endl;
			std::cout << msg << std::endl;
			Connection->writeLine("OK");
		}
	}
}
