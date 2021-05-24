/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageBuilder.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 14:24:42 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/03 15:00:46 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sstream>
#include "IrcNumeric.hpp"

/**
 * Message formatting tool.
 */
class MessageBuilder
{
public:
	/**
	 * Constructs a new custom message.
	 * \param prefix Origin of the message
	 * \param command Keyword used by client/server to identify the message
	 */
	MessageBuilder(const std::string &prefix, const std::string &command);

	/**
	 * Constructs a new message from an IrcNumeric.
	 * \param prefix Origin of the message
	 * \param numeric Structure which contains the data
	 * \param target Destination of the message
	 */
	MessageBuilder(const std::string &prefix, const IrcNumeric &numeric, const std::string &target);

	~MessageBuilder();

	/**
	 * \return The command of the message
	*/
	const std::string &command() const;

	/**
	 * \return The prefix of the message
	*/
	const std::string &prefix() const;

	/**
	 * \return The current length of the final formatted string
	*/
	size_t size() const;

	/**
	 * Convert the message into a string compatible with the IRC protocol which is :prefix arg*(' 'arg) [' ':last_arg].
	 * All empty arguments are replaced by an asterisk ('*').
	 * \return The string using the format required by the IRC protocol without newline
	*/
	std::string str() const;

	/**
	 * Inserts an argument at the end of the list.
	 * If the argument is empty and is not the last one, it will be replaced by '*'.
	 * Note that only the last argument will have ':'.
	 * Any arguments before the last containing  ' ' or ':' result in undefined behavior.
	 * \param arg The value to insert
	 * \return The MessageBuilder object (*this)
	*/
	MessageBuilder &operator<<(const std::string &arg);

	/**
	 * Inserts an argument at the end of the list.
	 * \param arg The value to insert
	 * \return The MessageBuilder object (*this)
	*/
	MessageBuilder &operator<<(unsigned arg);

private:
	const std::string	_prefix;
	const std::string	_command;
	std::stringstream	_args;
	std::string			_lastArg;
	bool				_lastArgSet;
	size_t				_size;

	MessageBuilder	&append(const std::string &arg);
};
