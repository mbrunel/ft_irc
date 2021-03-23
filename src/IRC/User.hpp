/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:43:09 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 14:57:53 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "RemoteServer.hpp"

class TcpSocket;

class User : public Mode
{
public:
	enum Flag
	{
		AWAY = 1,
		INVISIBLE = 1 << 2,
		WALLOPS = 1 << 3,
		RESTRICTED = 1 << 4,
		OPERATOR = 1 << 5,
		LOCAL_OPERATOR = 1 << 6,
		MARK = 1 << 7
	};

	enum State
	{
		NEED_PASS,
		NEED_NICK,
		NEED_USER,
		READY
	};

	User(TcpSocket *socket);
	virtual ~User();

	bool			isAway() const;
	RemoteServer	*makeRemoteServer();
	void			setState(const State state);
	State			state() const;

protected:
	TcpSocket	*_socket;
	std::string	_nickname;
	std::string _username;
	std::string _host;
	std::string _realname;
	unsigned	_servertoken;
	unsigned	_hopcount;
	State		_state;

private:
	User(const User &other);

	User	&operator=(const User &other);
};
