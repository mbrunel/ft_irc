#pragma once
#include <string>

namespace IRC
{

/**
 * Iterator tool used to parse message using the IRC protocol.
 */
class Context
{
public:
	/**
	 * Constructs a context.
	 * \param data A line to parse
	 */
	Context(const std::string &data);
	virtual ~Context();

	/**
	 * \return the length since the last call to resetDistance
	 */
	size_t distance() const;

	/**
	 * Extract an expresion between the last call to resetDistance and the current iterator.
	 * \return The substring between the 2 iterators
	 */
	std::string extract() const;

	/**
	 * \return true is the current character is not a space or ':' according to the IRC protocol
	 */
	bool isNospcrlfcl() const;

	/**
	 * \return true is the current character is a space according to the IRC protocol
	 */
	bool isSpace() const;

	/**
	 * \return true is the current character is special according to the IRC protocol
	 */
	bool isSpecial() const;

	/**
	 * Resets the current distance to parse an expression.
	 */
	void resetDistance();

	/**
	 * \return The current character or '\\0'
	 */
	char operator*() const;

	/**
	 * Iterates to the next character.
	 * \return itself
	 */
	Context &operator++();

	/**
	 * Iterates to the next character.
	 * \return itself
	 */
	Context operator++(int);

	Context &operator=(const Context &c);

private:
	typedef std::basic_string<char>::const_iterator	const_iterator;

	const std::string	&_data;
	const_iterator		_start, _i;
	size_t				_distance;
};

} /* end of namespace IRC */
