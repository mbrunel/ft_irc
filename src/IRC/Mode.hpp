/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 13:47:34 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/26 18:30:57 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

/**
 * Generic class using binary masks to define various boolean properties.
 */
class Mode
{
public:
	Mode(unsigned mask = 0);
	~Mode();

	/**
	 * Checks if all bits are set to true using a mask.
	 * \param mask The mask used to perform the check
	 * \return Positive if all bits from the mask are set to true
	*/
	bool areSet(unsigned mask) const;

	/**
	 * Checks if at least one bit is set to true using a mask.
	 * \param mask The mask used to perform the check
	 * \return Positive if at least one bit from the mask is set to true
	*/
	bool isSet(unsigned mask) const;

	/**
	 * Sets multiple bits to true using a mask.
	 * \param mask The mask used to set bits
	*/
	void set(unsigned mask);

	/**
	 * Sets multiple bits to false using a mask.
	 * \param mask The mask used to set bits
	*/
	void unset(unsigned mask);

private:
	unsigned _flags;
};
