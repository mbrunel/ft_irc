/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 13:02:49 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/11 22:07:08 by mbrunel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <stdint.h>
#include <netdb.h>
#include "exceptions.hpp"
#include "TcpSocket.hpp"

class Listener : public Socket
{
  public:
	Listener(int family);
	virtual ~Listener() throw();

	virtual TcpSocket*	accept();
	void				listen(int maxQueuLen = 3);

  private:
	Listener();
	Listener(const Listener &);
	Listener &operator=(const Listener &);
};
