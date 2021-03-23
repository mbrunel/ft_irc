/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 13:47:34 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 14:33:56 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Mode
{
public:
	Mode(unsigned flags = 0);
	virtual ~Mode();

	bool	isSet(unsigned flag) const;
	void	setFlags(unsigned flags);
	void	unsetFlags(unsigned flags);

private:
	unsigned	_flags;
};
