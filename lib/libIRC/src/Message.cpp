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

bool Message::parseMiddle(Context &o)
{
	o.resetDistance();
	if (!o.isNospcrlfcl())
		return (false);
	while ((++o).isNospcrlfcl() || *o == ':');
	_params.push_back(o.extract());
	return (true);
}

void Message::parseTrailing(Context &o)
{
	o.resetDistance();
	while (*o == ':' || *o == ' ' || o.isNospcrlfcl())
		++o;
	if (o.distance())
		_params.push_back(o.extract());
}

bool Message::interpret(Context &o)
{
	if ((*o == ':' && (!_prefix.interpret(++o) || *(o++) != ' ')) ||
	!Parser::asCommand(o, _command))
		return (reject());
	for (size_t i = 14; *o == ' '; --i)
		if (*(++o) == ':' || !i)
		{
			if (*o == ':')
				++o;
			parseTrailing(o);
			break ;
		}
		else if (!parseMiddle(o))
			return (reject());
	if ((*o != '\n' && (*o != '\r' || *(++o) != '\n')) || *(++o))
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
