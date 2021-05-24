/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RemoteUser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:46:03 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 22:19:36 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "User.hpp"

class RemoteUser : public User
{
public:
	RemoteUser(TcpSocket *socket, int hopcount);
	virtual ~RemoteUser();

private:
	RemoteUser(const RemoteUser &other);

	RemoteUser	&operator=(const RemoteUser &other);
};
