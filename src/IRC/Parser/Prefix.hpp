#pragma once
#include <iostream>
#include "Expression.hpp"

class Prefix : public Expression
{
public:
	Prefix();
	virtual ~Prefix();

	const std::string	&servername() const;
	const std::string	&nickname() const;
	const std::string	&user() const;
	const std::string	&host() const;
	bool				interpret(Context &o);

private:
	std::string	_servername;
	std::string	_nickname;
	std::string	_user;
	std::string	_host;
};

std::ostream	&operator<<(std::ostream &o, const Prefix &p);
