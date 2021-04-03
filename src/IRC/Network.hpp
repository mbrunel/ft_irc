/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Network.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 00:47:16 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/02 12:46:33 by asoursou         ###   ########.fr       */
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
	typedef std::map<std::string, Server *>				ServerMap;
	typedef std::map<std::string, Channel *>			ChannelMap;

	Network();
	~Network() throw();

	void			add(User *user);
	void			add(Server *server);
	void			add(Channel *channel);
	void			clear() throw();
	BasicConnection	*getBySocket(TcpSocket *socket);
	User			*getByNickname(const std::string &nickname);
	Server			*getByServername(const std::string &servername);
	Channel			*getByChannelname(const std::string &channelname);
	void			msgToAll(const std::string &msg, BasicConnection *origin = NULL);
	void			msgToChan(Channel *chan, const std::string &msg, User *origin = NULL);
	void			msgToNetwork(const std::string &msg, BasicConnection *origin = NULL);
	void			remove(User *user) throw();
	void			remove(Server *srv) throw();
	void			remove(const Channel *chan) throw();

  private:
	ConnectionMap	connections;
	UserMap			users;
	ServerMap		servers;
	ChannelMap		channels;

	Network(const Network &);
	Network &operator=(const Network &);
};
