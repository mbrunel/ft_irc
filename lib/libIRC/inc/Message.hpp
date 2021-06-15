#pragma once
#include "Param.hpp"
#include "Prefix.hpp"

namespace IRC
{

class Message : public Expression
{
public:
	typedef std::vector<Param> Params;

	Message(const std::string &entry);
	virtual ~Message();

	const std::string	&entry() const;
	const Prefix		&prefix() const;
	const std::string	&command() const;
	const Params		&params() const;

private:
	const std::string	_entry;
	Prefix				_prefix;
	std::string			_command;
	Params				_params;

	bool	parseMiddle(Context &o);
	void	parseTrailing(Context &o);
	bool	interpret(Context &o);
};

std::ostream	&operator<<(std::ostream &o, const Message &m);

} /* end of namespace IRC */
