#pragma once
#include <iostream>
#include "Expression.hpp"

namespace IRC
{

/**
 * The prefix of an IRC message.
 * \par Format:
 * servername | ( nickname [ [ '!' user ] '@' host ] )
 */
class Prefix : public Expression
{
public:
	Prefix();
	virtual ~Prefix();

	/**
	 * \return The servername (empty if not set)
	*/
	const std::string &servername() const;

	/**
	 * \return The nickname (empty if not set)
	*/
	const std::string &nickname() const;

	/**
	 * \return The user (empty if not set)
	*/
	const std::string &user() const;

	/**
	 * \return The host (empty if not set)
	*/
	const std::string &host() const;

	/**
	 * This function is called by the main expression and should not be used directly.
	 * \param c The context to use
	 * \return true if this expression is valid
	*/
	bool interpret(Context &c);

private:
	std::string _servername, _nickname, _user, _host;
};

std::ostream &operator<<(std::ostream &o, const Prefix &p);

} /* end of namespace IRC */
