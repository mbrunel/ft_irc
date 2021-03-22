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
#include "TcpSocket.hpp"

class RemoteServer
{
public:
	RemoteServer(TcpSocket *socket);
	virtual ~RemoteServer();

protected:
	TcpSocket	*_socket;

	RemoteServer(const RemoteServer &other);

	RemoteServer	&operator=(const RemoteServer &other);
};
