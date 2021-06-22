#include "Parser.hpp"
#include "Prefix.hpp"

namespace IRC
{

Prefix::Prefix()
{}

Prefix::~Prefix()
{}

bool Prefix::interpret(Context &c)
{
	Context oServername(c);

	Parser::asHostname(oServername, _servername);
	if (!Parser::asNickname(c, _nickname) && !_servername.size())
		return (reject());
	if (_servername.size() > _nickname.size())
	{
		c = oServername;
		_nickname.clear();
		return (accept());
	}
	_servername.clear();
	if (*c == '!' && !Parser::asUser(++c, _user))
		return (reject());
	if (*c == '@')
	{
		if (!Parser::asHost(++c, _host))
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

} /* end of namespace IRC */
