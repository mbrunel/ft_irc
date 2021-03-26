/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 13:47:34 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 18:30:41 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mode.hpp"

Mode::Mode(unsigned mask) :
_flags(mask)
{}

Mode::~Mode()
{}

bool Mode::areSet(unsigned mask) const
{
	return (!((_flags & mask) ^ mask));
}

bool Mode::isSet(unsigned mask) const
{
	return (!!(_flags & mask));
}

void Mode::set(unsigned mask)
{
	_flags |= mask;
}

void Mode::unset(unsigned mask)
{
	_flags &= ~mask;
}
