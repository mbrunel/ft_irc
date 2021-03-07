/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 12:39:08 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/07 13:10:11 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <netinet/in.h>

typedef struct sockaddr_in6	t_sockaddr_in6;
typedef struct sockaddr_in	t_sockaddr_in;
typedef struct sockaddr		t_sockaddr;

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
