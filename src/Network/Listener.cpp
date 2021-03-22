/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 00:54:33 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/14 14:58:41 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Listener.hpp"

Listener::Listener(int family):Socket() { socket(family); }

Listener::~Listener() throw() { close(); }

void Listener::listen(int maxQueueLen)
{
	if (::listen(_fd, maxQueueLen) == -1)
		throw ErrnoException("listen");
}

TcpSocket *Listener::accept() { return (new TcpSocket(_fd)); }
