/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 13:47:34 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 17:16:49 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

Mode::Mode(unsigned flags) :
_flags(flags)
{}

Mode::~Mode()
{}

bool Mode::isSet(unsigned flag) const
{
	return (!!(_flags & flag));
}

unsigned Mode::flags() const
{
	return (_flags);
}

void Mode::setFlags(unsigned flags)
{
	_flags |= flags;
}

void Mode::unsetFlags(unsigned flags)
{
	_flags &= ~flags;
}
