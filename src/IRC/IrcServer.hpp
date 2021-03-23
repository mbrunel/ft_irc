/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:57 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 22:01:17 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Message.hpp"
#include "NetworkModule.hpp"
#include "Sender.hpp"
#include "User.hpp"
#include <map>
#include <set>


class IrcServer
{
  public:
	typedef void (IrcServer::*cmdType)(Sender *sender, const Message &msg);
	typedef std::pair<std::string, Sender::Type> cmdIdType;
	typedef std::map<IrcServer::cmdIdType, cmdType> cmdMapType;
	typedef std::map<std::string, User *>::iterator nickIter;
	typedef std::map<TcpSocket *, User *>::iterator sockIter;

	IrcServer();
	~IrcServer();

	void			setMaxConnections(size_t setMaxConnections);
	void			setVerbose(bool verbose);
	void			setLogDestination(const std::string &destfile);
	void			listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);
	void			loadIrcCommands();

	void			userNick(Sender *sender, const Message &msg);
	void			userUser(Sender *sender, const Message &msg);
	void			privmsg(Sender *sender, const Message &msg);

	std::ostream	&log() throw();
	void			run() throw();

  private:
	TcpServer srv;
	std::map<std::string, User *>	allUsers;
	std::map<TcpSocket *, User *>	localUsers;
	cmdMapType						commands;

	void		loadCmd(const std::string &cmd, Sender::Type type, void (IrcServer::*handler)(Sender *sender, const Message &msg));
	User		*userFromSender(Sender *sender);
	Sender		*findSender(const Prefix &prefix, TcpSocket *Connection);
	void		exec(Sender *sender, const Message &msg);
	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
