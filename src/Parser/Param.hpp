/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Param.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:24:26 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/26 13:29:09 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <vector>
#include "Context.hpp"

class Param : public std::string
{
public:
	Param(const std::string &value);
	virtual ~Param();

	bool				isChannel() const;
	bool				isMask() const;
	bool				isNickname() const;
	bool				isKey() const;
	std::string			mask() const;
	std::vector<Param>	split(char delimiter = ',') const;

private:
	bool	check(bool (*parsing_func)(Context &, std::string &)) const;
};
