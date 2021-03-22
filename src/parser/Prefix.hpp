/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prefix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 13:40:12 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/19 18:34:20 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "Expression.hpp"

class Prefix : public Expression
{
public:
	Prefix();
	virtual ~Prefix();

	const std::string	&servername() const;
	const std::string	&nickname() const;
	const std::string	&user() const;
	const std::string	&host() const;
	bool				interpret(Context &o);

private:
	std::string	_servername;
	std::string	_nickname;
	std::string	_user;
	std::string	_host;
};

std::ostream	&operator<<(std::ostream &o, const Prefix &p);
