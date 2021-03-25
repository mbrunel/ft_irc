/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:57 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/25 11:43:11 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Channel.hpp"
#include "Message.hpp"
#include "NetworkModule.hpp"
#include "BasicConnection.hpp"
#include "User.hpp"
#include <map>
#include <set>


class IrcServer
{
  public:
	typedef void (IrcServer::*cmdType)(BasicConnection *sender, const Message &msg);
	typedef std::pair<std::string, BasicConnection::Type> cmdIdType;
	typedef std::map<IrcServer::cmdIdType, cmdType> cmdMapType;
	typedef std::map<std::string, User *>::iterator nickIter;
	typedef std::map<TcpSocket *, User *>::iterator sockIter;
	typedef std::map<std::string, Channel> channelsMap;
	typedef std::list<Param> Params;

	IrcServer();
	~IrcServer();

	void			setMaxConnections(size_t setMaxConnections);
	void			setVerbose(bool verbose);
	void			setLogDestination(const std::string &destfile);
	void			listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);
	void			loadIrcCommands();

	void			join(BasicConnection *sender, const Message &msg);
	void			privmsg(BasicConnection *sender, const Message &msg);
	void			userNick(BasicConnection *sender, const Message &msg);
	void			userUser(BasicConnection *sender, const Message &msg);

	void			disconnect(TcpSocket *socket) throw();
	void			disconnect(BasicConnection *connection) throw();
	std::ostream	&log() throw();
	void			run() throw();

  private:
	TcpServer srv;
	std::map<std::string, User *>	allUsers;
	std::map<TcpSocket *, User *>	localUsers;
	channelsMap						channels;
	cmdMapType						commands;
	BasicConnection					*sender;
	Message							msg;

	void				loadCmd(const std::string &cmd, BasicConnection::Type type, void (IrcServer::*handler)(BasicConnection *sender, const Message &msg));
	User				*userFromConnection(BasicConnection *sender);
	BasicConnection		*findSender(const Prefix &prefix, TcpSocket *Connection);
	void				exec(BasicConnection *sender, const Message &msg);
	bool				checkParams(size_t min);
	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
