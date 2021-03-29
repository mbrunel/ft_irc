/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 00:47:16 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/29 19:08:30 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <assert.h>
#include <map>
#include "Channel.hpp"
#include "RemoteServer.hpp"

class Network
{
  public:
	typedef std::map<TcpSocket *, BasicConnection *>	ConnectionMap;
	typedef std::map<std::string, User *>				UserMap;
	typedef std::map<std::string, RemoteServer *>		ServerMap;
	typedef std::map<std::string, Channel *>			ChannelMap;

	Network();
	~Network() throw();

	void			add(User *user);
	void			add(RemoteServer *server);
	void			add(Channel *channel);
	void			clear() throw();
	BasicConnection	*getBySocket(TcpSocket *socket);
	User			*getByNickname(const std::string &nickname);
	RemoteServer	*getByServername(const std::string &servername);
	Channel			*getByChannelname(const std::string &channelname);
	void			msgToNetwork(const std::string &msg);
	void			remove(User *user) throw();
	void			remove(RemoteServer *srv) throw();
	void			remove(const Channel *chan) throw();
	void			msgToChan(Channel *chan, const std::string &msg, User * sender = NULL);

  private:
	ConnectionMap	connections;
	UserMap			users;
	ServerMap		servers;
	ChannelMap		channels;

	Network(const Network &);
	Network &operator=(const Network &);
};
