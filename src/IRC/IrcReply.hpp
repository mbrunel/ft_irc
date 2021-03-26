/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcReply.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 12:56:43 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 13:10:10 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

namespace IrcReply
{
	std::string away(const std::string &prefix, const std::string &nickname, const std::string &raison);
	std::string unaway(const std::string &prefix);
	std::string noaway(const std::string &prefix);
}
