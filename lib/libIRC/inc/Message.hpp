#pragma once
#include "Param.hpp"
#include "Prefix.hpp"

namespace IRC
{

/**
 * Parse any message using the IRC protocol.
 * \par Format:
 * [ ':' prefix hspace ] command [ params ] "\r\n"
 */
class Message : public Expression
{
public:
	typedef std::vector<Param> Params;

	/**
	 * Constructs a message.
	 * \param entry A line ending with "\n" or "\r\n" 
	 */
	Message(const std::string &entry);

	virtual ~Message();

	/**
	 * \return Return raw entry
	*/
	const std::string &entry() const;

	/**
	 * \return Return the optional prefix
	*/
	const Prefix &prefix() const;

	/**
	 * \return Return the command which is an alphabetic string or 3 digits
	*/
	const std::string &command() const;

	/**
	 * \return Return optional arguments
	*/
	const Params &params() const;

private:
	const std::string	_entry;
	Prefix				_prefix;
	std::string			_command;
	Params				_params;

	bool parseMiddle(Context &c);
	void parseTrailing(Context &c);
	bool interpret(Context &c);
};

std::ostream &operator<<(std::ostream &o, const Message &m);

} /* end of namespace IRC */
