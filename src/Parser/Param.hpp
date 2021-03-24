/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:24:26 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/24 13:13:57 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <list>
#include "MsgTo.hpp"

class Param : public std::string
{
public:
	typedef std::list<Param>	t_ParamList;

	Param(const std::string &value);
	virtual ~Param();

	MsgTo		asMsgTo() const;
	std::string	asNickname() const;
	t_ParamList	split(char delimiter = ',') const;
};
