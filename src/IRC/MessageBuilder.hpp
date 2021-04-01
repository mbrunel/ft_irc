/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:24:42 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 18:57:03 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "IrcNumeric.hpp"

namespace MessageBuilder
{
	std::string create(const std::string &src, const std::string &dst, const std::string &command, const std::string &content = "");
	std::string create(const std::string &src, const std::string &dst, IrcNumeric response);
}
