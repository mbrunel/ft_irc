/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 21:04:26 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/01 21:06:14 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "BasicConnection.hpp"

class Server : public BasicConnection
{
  public:
	Server(TcpSocket *socket);
	virtual ~Server();

	void setName(const std::string &name);
	const std::string &name() const;

  private:

	std::string _name;

	Server(const Server &other);
	Server	&operator=(const Server &other);
};
