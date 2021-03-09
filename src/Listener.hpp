/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 13:02:49 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/07 15:02:31 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdint.h>
#include "TcpSocket.hpp"

class Listener : public Socket
{
  public:
	Listener(uint16_t port, size_t maxConnections = 1024);
	virtual ~Listener();

	size_t				maxConnections() const;
	virtual TcpSocket*	accept();

  protected:
	size_t	_maxConnections;

	Listener(const Listener &);
	Listener &operator=(const Listener &);
};
