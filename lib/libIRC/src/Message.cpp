#include "Message.hpp"
#include "Parser.hpp"

namespace IRC
{

Message::Message(const std::string &entry) : _entry(entry)
{
	Context o(entry);

	if (entry.size() <= 512)
		interpret(o);
}

Message::~Message()
{}

const std::string &Message::entry() const
{
	return (_entry);
}

const Prefix &Message::prefix() const
{
	return (_prefix);
}

const std::string &Message::command() const
{
	return (_command);
}

const Message::Params &Message::params() const
{
	return (_params);
}

bool Message::parseMiddle(Context &c)
{
	c.resetDistance();
	if (!c.isNospcrlfcl())
		return (false);
	while ((++c).isNospcrlfcl() || *c == ':');
	_params.push_back(c.extract());
	return (true);
}

void Message::parseTrailing(Context &c)
{
	c.resetDistance();
	while (*c == ':' || *c == ' ' || c.isNospcrlfcl())
		++c;
	if (c.distance())
		_params.push_back(c.extract());
}

bool Message::interpret(Context &c)
{
	if ((*c == ':' && (!_prefix.interpret(++c) || *(c++) != ' ')) ||
	!Parser::asCommand(c, _command))
		return (reject());
	for (size_t i = 14; *c == ' '; --i)
		if (*(++c) == ':' || !i)
		{
			if (*c == ':')
				++c;
			parseTrailing(c);
			break ;
		}
		else if (!parseMiddle(c))
			return (reject());
	if ((*c != '\n' && (*c != '\r' || *(++c) != '\n')) || *(++c))
		return (reject());
	return (accept());
}

std::ostream &operator<<(std::ostream &o, const IRC::Message &m)
{
	o << '[';
	if (!m.isValid())
		return (o << ']');
	if (m.prefix().isValid())
		o << "prefix: " << m.prefix() << ", ";
	o << "command: " << m.command();
	if (m.params().size())
	{
		o << ", params: [";
		std::vector<Param>::const_iterator i = m.params().begin();
		o << *i;
		for (++i; i != m.params().end(); ++i)
			o << ", " << *i;
		o << ']';
	}
	return (o << ']');
}

} /* end of namespace IRC */
