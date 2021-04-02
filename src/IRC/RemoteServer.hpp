/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RemoteRemoteServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:43:09 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/22 12:54:16 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Server.hpp"

class RemoteServer : public Server
{
public:
	RemoteServer(TcpSocket *socket, int hopcount);
	virtual ~RemoteServer();

private:
	RemoteServer(const RemoteServer &other);

	RemoteServer	&operator=(const RemoteServer &other);
};
