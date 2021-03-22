/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RemoteUser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 12:46:03 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/22 13:15:50 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "User.hpp"

class RemoteUser : public User
{
public:
	RemoteUser(TcpSocket *socket);
	virtual ~RemoteUser();

private:
	RemoteUser(const RemoteUser &other);

	RemoteUser	&operator=(const RemoteUser &other);
};
