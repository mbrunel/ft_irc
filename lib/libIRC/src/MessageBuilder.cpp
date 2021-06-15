#include <iomanip>
#include "MessageBuilder.hpp"
#include "Utils.hpp"

static inline std::string numToString(const IRC::NumericCode code)
{
	std::ostringstream s;
	s << std::setw(3) << std::setfill('0') << code;
	return (s.str());
}

namespace IRC
{

MessageBuilder::MessageBuilder(const std::string &prefix, const std::string &command) :
_prefix(prefix),
_command(command),
_lastArgSet(false),
_size(prefix.size() + command.size() + 2)
{}

MessageBuilder::MessageBuilder(const std::string &prefix, const IRC::Numeric &numeric, const std::string &target) :
_prefix(prefix),
_command(numToString(numeric.code)),
_lastArgSet(false),
_size(prefix.size() + 5)
{
	_args << ' ' << (target.size() ? target : "*") << ' ' << numeric.content;
	_size += _args.str().size();
}

MessageBuilder::~MessageBuilder()
{}

const std::string &MessageBuilder::command() const
{
	return (_command);
}

const std::string &MessageBuilder::prefix() const
{
	return (_prefix);
}

size_t MessageBuilder::size() const
{
	return (_size + (_lastArgSet ? _lastArg.size() + 2 : 0));
}

std::string MessageBuilder::str() const
{
	std::stringstream s;
	s << ':' << _prefix << ' ' << _command << _args.str();
	if (_lastArgSet)
		s << " :" << _lastArg;
	return (s.str());
}

MessageBuilder &MessageBuilder::operator<<(const std::string &arg)
{
	return (append(arg));
}

MessageBuilder &MessageBuilder::operator<<(unsigned arg)
{
	return (append(Utils::to_string(arg)));
}

MessageBuilder &MessageBuilder::append(const std::string &arg)
{
	if (_lastArgSet)
	{
		if (! _lastArg.size())
			_lastArg = '*';
		_args << ' ' << _lastArg;
		_size += _lastArg.size() + 1;
	}
	else
		_lastArgSet = true;
	_lastArg = arg;
	return (*this);
}

} /* end of namespace IRC */
