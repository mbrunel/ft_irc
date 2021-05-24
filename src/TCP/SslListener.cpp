/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SslListener.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 02:11:27 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/15 00:50:54 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SslListener.hpp"

SslListener::SslListener(int family, SSL_CTX *ctx):Listener(family), _ctx(ctx) {}

SslListener::~SslListener() throw() {}

TcpSocket *SslListener::accept() { return (new SslSocket(_fd, _ctx)); }
