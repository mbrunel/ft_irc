/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:57 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/01 21:09:27 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Network.hpp"
#include "TcpServer.hpp"
#include "Message.hpp"

struct IrcServerConfig
{
	std::string	version;
	size_t		maxChannels;

	IrcServerConfig(const std::string &version, size_t maxChannels = 0);
};

class IrcServer
{
  public:
	typedef std::map<std::string, void (IrcServer::*)(User &, const Message &)> userCommandsMap;
	typedef std::map<std::string, void (IrcServer::*)(Server &, const Message &)> serverCommandsMap;
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
	std::string			prefix;

	void	away(User &sender, const Message &msg);
	void	join(User &sender, const Message &msg);
	void	motd(User &sender, const Message &msg);
	void	nick(User &sender, const Message &msg);
	void	part(User &sender, const Message &msg);
	void	privmsg(User &sender, const Message &msg);
	void	topic(User &sender, const Message &msg);
	void	user(User &sender, const Message &msg);

	void	disconnect(TcpSocket *socket) throw();
	void	disconnect(User *connection) throw();
	void	disconnect(Server *connection) throw();
	void	exec(BasicConnection *sender, const Message &msg);
	void	writeNum(User &dst, const IrcNumeric &response);
	void	writeWelcome(User &user);
	void	writeMotd(User &user);

	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
