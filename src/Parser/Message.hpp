/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 13:40:12 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/29 19:37:30 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "Param.hpp"
#include "Prefix.hpp"

class Message : public Expression
{
public:
	Message(const std::string &entry);
	virtual ~Message();

	const std::string			&entry() const;
	const Prefix				&prefix() const;
	const std::string			&command() const;
	const std::vector<Param>	&params() const;

private:
	const std::string	_entry;
	Prefix				_prefix;
	std::string			_command;
	std::vector<Param>	_params;

	bool	parseMiddle(Context &o);
	void	parseTrailing(Context &o);
	bool	interpret(Context &o);
};

std::ostream	&operator<<(std::ostream &o, const Message &m);
