#pragma once
#include <vector>
#include "Context.hpp"

namespace IRC
{

/**
 * The param of an IRC message.
 */
class Param : public std::string
{
public:
	/**
	 * Constructs an empty param.
	 */
	Param();

	/**
	 * Constructs a param from a std::string.
	 * \param value The value of the param
	 */
	Param(const std::string &value);

	virtual ~Param();

	/**
	 * \return true if this param is a channel name
	 */
	bool isChannel() const;

	/**
	 * \return true if this param is a mask (contains at least one '*' or '?')
	 */
	bool isMask() const;

	/**
	 * \return true if this param is a nickname
	 */
	bool isNickname() const;

	/**
	 * \return true if this param is a channel key
	 */
	bool isKey() const;

	/**
	 * \return The reduced form of a mask ("***" becomes "*")
	 */
	std::string mask() const;

	/**
	 * Split this param into a new set of arguments.
	 * \param delimiter The delimiter to use to split the string
	 * \return The new sequence of arguments
	 */
	std::vector<Param> split(char delimiter = ',') const;

private:
	bool check(bool (*parsingFunc)(Context &, std::string &)) const;
};
} /* end of namespace IRC */
