/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/07 11:30:18 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/19 20:24:02 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Message.hpp"

int main(int ac, char **av)
{
	if (ac == 2)
	{
		Message msg(std::string(av[1]) + '\n');

		if (msg.isValid())
			std::cout << msg << std::endl;
		else
			std::cerr << "error: Invalid message" << std::endl;
	}
	return (0);
}
