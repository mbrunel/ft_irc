/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:57 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/27 15:31:17 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "BasicConnection.hpp"
#include "Channel.hpp"
#include "Message.hpp"
#include "TcpServer.hpp"

struct IrcServerConfig
{
	size_t	maxChannels;

	IrcServerConfig(size_t maxChannels = 0);
};

class IrcServer
{
  public:
	typedef void (IrcServer::*cmdType)(BasicConnection *sender, const Message &msg);
	typedef std::pair<std::string, BasicConnection::Type> cmdIdType;
	typedef std::map<IrcServer::cmdIdType, cmdType> cmdMapType;
	typedef std::map<std::string, User *>::iterator nickIter;
	typedef std::map<TcpSocket *, User *>::iterator sockIter;
	typedef std::map<std::string, Channel> ChannelMap;
	typedef std::list<Param> Params;

	IrcServer();
	~IrcServer();

	void			setMaxConnections(size_t setMaxConnections);
	void			setVerbose(bool verbose);
	void			setLogDestination(const std::string &destfile);
	void			listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);

	void			away(BasicConnection *sender, const Message &msg);
	void			join(BasicConnection *sender, const Message &msg);
	void			privmsg(BasicConnection *sender, const Message &msg);
	void			topic(BasicConnection *sender, const Message &msg);
	void			userNick(BasicConnection *sender, const Message &msg);
	void			userUser(BasicConnection *sender, const Message &msg);

	void			disconnect(TcpSocket *socket) throw();
	void			disconnect(BasicConnection *connection) throw();
	std::ostream	&log() throw();
	void			run() throw();

  private:
	const IrcServerConfig			config;
	TcpServer						srv;
	std::map<std::string, User *>	allUsers;
	std::map<TcpSocket *, User *>	localUsers;
	ChannelMap						channels;
	cmdMapType						commands;

	void				broadcast(const Channel &channel, const std::string &message, User *except = NULL);
	void				load(const std::string &cmd, BasicConnection::Type type, void (IrcServer::*handler)(BasicConnection *sender, const Message &msg));
	User				*userFromConnection(BasicConnection *sender);
	BasicConnection		*findSender(const Prefix &prefix, TcpSocket *Connection);
	void				exec(BasicConnection *sender, const Message &msg);
	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
