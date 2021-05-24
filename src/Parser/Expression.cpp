/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expression.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 19:25:01 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 18:21:54 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Expression.hpp"

Expression::Expression() : _isValid(false)
{}

Expression::~Expression()
{}

bool Expression::isValid() const
{
	return (_isValid);
}

bool Expression::accept()
{
	return (_isValid = true);
}

bool Expression::reject()
{
	return (_isValid = false);
}
