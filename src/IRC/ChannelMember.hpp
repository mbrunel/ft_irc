/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelMember.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/22 18:40:20 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/22 18:41:00 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class ChannelMember
{
public:
	ChannelMember();
	~ChannelMember();

private:
	ChannelMember(const ChannelMember &other);

	ChannelMember	&operator=(const ChannelMember &other);
};
