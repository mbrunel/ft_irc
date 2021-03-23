/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sender.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 18:50:36 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/23 21:21:31 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "TcpSocket.hpp"

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
	TcpSocket	*_socket;
};
