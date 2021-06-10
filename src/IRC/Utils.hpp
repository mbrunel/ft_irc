/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/08 18:40:30 by asoursou          #+#    #+#             */
/*   Updated: 2021/06/10 18:52:03 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sstream>
#include <string>

namespace Utils
{
	bool match(const std::string &mask, const std::string &str);

	std::string to_date(const time_t t,  const char *format);

	template<class T>
	inline std::string to_string(const T &value)
	{
		std::stringstream stringbuf;
		stringbuf << value;
		return (stringbuf.str());
	}
}
