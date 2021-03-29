/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:57 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/29 19:36:37 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Network.hpp"
#include "TcpServer.hpp"
#include "Message.hpp"

struct IrcServerConfig
{
	size_t	maxChannels;

	IrcServerConfig(size_t maxChannels = 0);
};

class IrcServer
{
  public:
	typedef std::map<std::string, void (IrcServer::*)(User &, const Message &)> userCommandsMap;
	typedef std::map<std::string, void (IrcServer::*)(RemoteServer &, const Message &)> serverCommandsMap;
	typedef std::vector<Param> Params;

	IrcServer();
	~IrcServer();

	void			listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);
	std::ostream	&log() throw();
	void			run() throw();
	void			setLogDestination(const std::string &destfile);
	void			setMaxConnections(size_t setMaxConnections);
	void			setVerbose(bool verbose);

  private:
	IrcServerConfig		config;
	TcpServer			srv;
	Network				network;
	userCommandsMap		userCommands;
	serverCommandsMap	serverCommands;

	void			away(User &sender, const Message &msg);
	void			join(User &sender, const Message &msg);
	void			privmsg(User &sender, const Message &msg);
	void			topic(User &sender, const Message &msg);
	void			nick(User &sender, const Message &msg);
	void			user(User &sender, const Message &msg);

	void			broadcast(const Channel &channel, const std::string &message, User *except = NULL);
	void			disconnect(TcpSocket *socket) throw();
	void			disconnect(User *connection) throw();
	void			disconnect(RemoteServer *connection) throw();
	void			exec(BasicConnection *sender, const Message &msg);

	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
