/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prefix.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 19:40:19 by asoursou          #+#    #+#             */
/*   Updated: 2021/03/23 18:26:07 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Prefix.hpp"

Prefix::Prefix()
{}

Prefix::~Prefix()
{}

bool Prefix::interpret(Context &o)
{
	Context oServername(o);

	Parser::asHostname(oServername, _servername);
	if (!Parser::asNickname(o, _nickname) && !_servername.size())
		return (reject());
	if (_servername.size() > _nickname.size())
	{
		o = oServername;
		_nickname.clear();
		return (accept());
	}
	_servername.clear();
	if (*o == '!' && !Parser::asUser(++o, _user))
		return (reject());
	if (*o == '@')
	{
		if (!Parser::asHost(++o, _host))
			return (reject());
	}
	else if (_user.size())
			return (reject());
	return (accept());
}


const std::string &Prefix::servername() const
{
	return (_servername);
}

const std::string &Prefix::nickname() const
{
	return (_nickname);
}

const std::string &Prefix::user() const
{
	return (_user);
}

const std::string &Prefix::host() const
{
	return (_host);
}

std::ostream &operator<<(std::ostream &o, const Prefix &p)
{
	o << '[';
	if (!p.isValid())
		return (o << ']');
	else if (p.servername().size())
		return (o << "servername: " << p.servername() << ']');
	o << "nickname: " << p.nickname();
	if (p.user().size())
		o << ", user: " << p.user();
	if (p.host().size())
		o << ", host: " << p.host();
	return (o << ']');
}
