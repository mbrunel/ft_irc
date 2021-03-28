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
#include "BasicConnection.hpp"

class RemoteServer : public BasicConnection
{
  public:
	RemoteServer(TcpSocket *socket);
	virtual ~RemoteServer();

	void setName(const std::string &name);
	const std::string &name() const;

  private:

	std::string _name;

	RemoteServer(const RemoteServer &other);
	RemoteServer	&operator=(const RemoteServer &other);
};
