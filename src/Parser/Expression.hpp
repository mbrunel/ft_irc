/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expression.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 19:25:46 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/20 16:09:10 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Context.hpp"

class Expression
{
public:
	Expression();
	virtual ~Expression();

	bool			isValid() const;
	virtual bool	interpret(Context &o) = 0;

protected:
	bool	accept();
	bool	reject();
	bool	asCommand(Context &o, std::string &s) const;
	bool	asHost(Context &o, std::string &s) const;
	bool	asHostname(Context &o, std::string &s) const;
	bool	asNickname(Context &o, std::string &s) const;
	bool	asUser(Context &o, std::string &s) const;

private:
	bool	_isValid;

	bool	isNumber(Context &o, size_t min, size_t max) const;
	bool	isXNumber(Context &o, size_t min, size_t max) const;
	bool	isIPv4(Context &o) const;
	bool	isIPv6(Context &o) const;
	bool	isShortname(Context &o) const;
};
