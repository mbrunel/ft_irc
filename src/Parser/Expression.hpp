/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Expression.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 19:25:46 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 18:17:55 by asoursou         ###   ########.fr       */
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

private:
	bool	_isValid;
};
