/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sender.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 18:50:36 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 22:22:47 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "TcpSocket.hpp"

// class that allows to communicate with an unknown connection

class Sender
{
  public:
	enum Type {USER, SERVER};

	Sender(Type type, TcpSocket *socket);
	virtual ~Sender();

	Type type() const;
	void writeTo(const std::string &);

  private:
	Type		_type;
	TcpSocket	*_socket; //only here and not in user anymore
};
