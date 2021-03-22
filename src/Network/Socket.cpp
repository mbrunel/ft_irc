/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/08 01:37:26 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/14 14:59:45 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket():_fd(-1) {}

Socket::~Socket() throw() {}

int Socket::fd() const { return (_fd); }

const sockaddr_storage &Socket::addr() const { return (_addr); }

sa_family_t Socket::family() const { return (_addr.ss_family); }

void Socket::socket(int family)
{
	if ((_fd = ::socket(family, SOCK_STREAM, IPPROTO_TCP)) == -1)
		throw ErrnoException("socket");
}

int Socket::close() throw() { return (::close(_fd)); }

void Socket::bind(sockaddr *addr, socklen_t addr_size)
{
	if (::bind(_fd, addr, addr_size) == -1)
		throw ErrnoException("bind");
	_addr = *(sockaddr_storage *)addr;
}

void Socket::setReuseAddr()
{
	int reuse = 1;
	if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
		throw ErrnoException("setsockopt");
}

void Socket::setIpv6only()
{
	int v6Only = 1;
	if (::setsockopt(_fd, IPPROTO_IPV6, IPV6_V6ONLY, &v6Only, sizeof(v6Only)) == -1)
		throw ErrnoException("setsockopt");
}
void Socket::setNonblock()
{
	if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
		throw ErrnoException("fcntl");
}
