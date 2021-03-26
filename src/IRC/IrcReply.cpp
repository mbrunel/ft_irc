/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcReply.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/27 14:00:28 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/26 13:10:15 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

namespace IrcReply
{
	std::string away(const std::string &prefix, const std::string &nickname, const std::string &raison)
	{
		return (':' + prefix + " 301 " + nickname + " :" + raison);
	}

	std::string unaway(const std::string &prefix)
	{
		return (':' + prefix + " 305 :You are no longer marked as being away");
	}

	std::string noaway(const std::string &prefix)
	{
		return (':' + prefix + " 306 :You have been marked as being away");
	}
}
