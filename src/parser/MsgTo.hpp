/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MsgTo.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 19:46:13 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/19 20:16:44 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include "Expression.hpp"

class MsgTo : public Expression
{
public:
	enum Type { CHANNEL, NICKNAME, TARGETMASK };

	MsgTo();
	virtual ~MsgTo();

	Type				type() const;
	const std::string	&target() const;
	bool				interpret(Context &o);

private:
	Type		_type;
	std::string	_target;
};

std::ostream	&operator<<(std::ostream &o, const MsgTo &m);
