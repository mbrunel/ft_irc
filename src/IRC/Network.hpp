/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 00:47:16 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/28 21:06:49 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <list>
#include <assert.h>

#include "BasicConnection.hpp"
#include "Channel.hpp"
#include "User.hpp"
#include "RemoteServer.hpp"

class Network
{
  public:
	typedef std::map<std::string, User *> UserMap;
	typedef std::map<std::string, RemoteServer *> ServerMap;
	typedef std::map<TcpSocket *, BasicConnection *> LocalMap;
	typedef std::map<std::string, Channel *> ChannelMap;

	Network();
	~Network() throw();

	User			*newUser(BasicConnection *connection);
	RemoteServer	*newServer(BasicConnection *connection);
	BasicConnection	*newConnection(TcpSocket *socket);
	Channel			*newChan(const std::string &name);
	void			addUser(User *user);
	void			addServer(RemoteServer *srv);
	void			addConnection(BasicConnection *connection);
	void			addChan(Channel *chan);
	bool			isUser(const std::string &nick);
	bool			isServer(const std::string &name);
	bool			isConnection(TcpSocket *socket);
	bool			isChan(const std::string &name);
	User			*getUser(const std::string &nick);
	RemoteServer	*getServer(const std::string &srvname);
	Channel			*getChan(const std::string &name);
	BasicConnection	*getConnection(TcpSocket *socket);
	void			removeUser(const std::string &nick) throw();
	void			removeUser(User *user) throw();
	void			removeServer(const std::string &srvname) throw();
	void			removeServer(RemoteServer *srv) throw();
	void			removeConnection(TcpSocket *socket) throw();
	void			removeConnection(BasicConnection *connection) throw();
	void			removeChan(const std::string &name) throw();
	void			removeChan(const Channel *chan) throw();
	void			clear() throw();
	User			*conToUsr(BasicConnection *connection);
	RemoteServer	*conToSrv(BasicConnection *connection);
	void			msgToNetwork(const std::string &msg);
	void			msgToChan(Channel *chan, const std::string &msg, User * sender = NULL);

  private:
	UserMap		users;
	LocalMap	connections;
	ChannelMap	channels;
	ServerMap	servers;

	Network(const Network &);
	Network &operator=(const Network &);
};
