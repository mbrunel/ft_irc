/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:24:26 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 18:09:58 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <list>
#include "MsgTo.hpp"

class Param : public std::string
{
public:
	Param(const std::string &value);
	virtual ~Param();

	MsgTo				asMsgTo() const;
	std::list<MsgTo>	asMultipleMsgTo() const;
	std::string			asNickname() const;
};
