/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:24:26 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/29 19:38:20 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include "MsgTo.hpp"

class Param : public std::string
{
public:
	Param(const std::string &value);
	virtual ~Param();

	std::string			asChannel() const;
	MsgTo				asMsgTo() const;
	std::string			asNickname() const;
	std::string			asKey() const;
	std::vector<Param>	split(char delimiter = ',') const;

private:
	std::string	parse(bool (*parsing_func)(Context &, std::string &)) const;
};
