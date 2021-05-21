/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 23:31:57 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/21 15:59:21 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IrcNumeric.hpp"
#include "Network.hpp"
#include "TcpServer.hpp"
#include "Message.hpp"
#include "Config.hpp"

struct IrcServerConfig
{
	std::string	version;
	std::string servername;
	size_t		maxChannels;
	time_t		ping;
	time_t		pong;
	bool		flood;

	IrcServerConfig();
	IrcServerConfig(Config &cfg);
};

class IrcServer
{
  public:
	IrcServer();
	~IrcServer();

	void			listen(const char *port, SSL_CTX *ctx = NULL, size_t maxQueueLen = 3);
	std::ostream	&log() throw();
	void			run() throw();
	void			setConfig(Config &cfg);

  private:
	typedef int (IrcServer::*UserCommandPointer)(User &, const Message &);
	typedef int (IrcServer::*ServerCommandPointer)(Server &, const Message &);
	typedef std::map<std::string, UserCommandPointer> userCommandsMap;
	typedef std::map<std::string, ServerCommandPointer> serverCommandsMap;
	typedef std::vector<Param> Params;

	IrcServerConfig		config;
	TcpServer			srv;
	Network				network;
	userCommandsMap		userCommands;
	serverCommandsMap	serverCommands;

	int	away(User &sender, const Message &msg);
	int	join(User &sender, const Message &msg);
	int mode(User &sender, const Message &msg);
	int	motd(User &sender, const Message &msg);
	int	nick(User &sender, const Message &msg);
	int	part(User &sender, const Message &msg);
	int	privmsg(User &sender, const Message &msg);
	int	topic(User &sender, const Message &msg);
	int	user(User &sender, const Message &msg);
	int oper(User &sender, const Message &msg);
	int ping(User &sender, const Message &msg);
	int pong(User &sender, const Message &msg);

	void	disconnect(TcpSocket *socket) throw();
	void	disconnect(User *connection) throw();
	void	disconnect(Server *connection) throw();
	int		exec(BasicConnection *sender, const Message &msg);
	void	writeMessage(User &dst, const std::string &command, const std::string &content);
	int		writeNum(User &dst, const IrcNumeric &response);
	void	writeWelcome(User &user);
	void	writeMotd(User &user);
	void	Police();
	bool	floodControl(User &u);

	IrcServer(const IrcServer& copy);
	IrcServer &operator=(const IrcServer& copy);
};
