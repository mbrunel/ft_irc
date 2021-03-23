/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:48 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 14:43:07 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

IrcServer::IrcServer() {}

IrcServer::~IrcServer() {}

void IrcServer::setMaxConnections(size_t MaxConnections) { srv.setMaxConnections(MaxConnections); }

void IrcServer::setVerbose(bool verbose) { srv.setVerbose(verbose); }

void IrcServer::setLogDestination(const std::string &destfile) { srv.setLogDestination(destfile); }

std::ostream &IrcServer::log() throw() { return srv.log(); }

void IrcServer::listen(const char *port, SSL_CTX *ctx, size_t maxQueueLen) { srv.listen(port, ctx, maxQueueLen); }

void IrcServer::run() throw()
{
	while (1)
	{
		try { srv.select(); } catch(TcpServer::SigintException &e) { log() << "\rSIGINT catched, exiting properly" << std::endl; return ; }
		TcpSocket *newConnection;
		while ((newConnection = srv.nextNewConnection()))
			newConnection->writeLine("BONJOUR CHER CLIENT");
		TcpSocket *Connection;
		while ((Connection = srv.nextPendingConnection()))
		{
			try { if (!Connection->IO()) { srv.disconnect(Connection); continue ; } }
			catch (std::exception &e) { log() << e.what() << std::endl; srv.disconnect(Connection); continue ; }
			std::string line;
			if (!Connection->readLine(line))
				continue ;
			log() << line;
			Message msg(line);
			log() << msg.isValid() << " : " << msg << std::endl;
			Connection->writeLine("OK");
		}
	}
}
