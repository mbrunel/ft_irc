/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SslContext.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 01:18:20 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/14 14:56:44 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <openssl/ssl.h>
#include "exceptions.hpp"

struct SslContext
{
  public :
	SslContext(const char *certificatePath, const char *keyPath);
	~SslContext() throw();

	SSL_CTX *ctx();

  private :
	SSL_CTX *_ctx;
};
