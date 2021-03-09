/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 12:39:08 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/08 19:40:45 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <netinet/in.h>

typedef struct sockaddr_in6	t_sockaddr_in6;
typedef struct sockaddr_in	t_sockaddr_in;
typedef struct sockaddr		t_sockaddr;

/**
 * A generic two-way connection-based socket both compatible with IPv4 and IPv6 network layer
 */
class Socket
{
  public:
	Socket();
	Socket(int fd);
	virtual ~Socket();

  protected:
	int				_fd;
	t_sockaddr_in6	_addr;

  private:
	Socket(const Socket &other);
	Socket &operator=(const Socket &other);
};
