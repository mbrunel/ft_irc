#include "MessageBuilder.hpp"
#include "User.hpp"

UserRequirement::UserRequirement(unsigned flags) :
Mode(flags)
{}

UserRequirement::~UserRequirement()
{}

UserMode::UserMode(unsigned flags) :
Mode(flags)
{}

UserMode::~UserMode()
{}

UserMode::Flag UserMode::parse(char c)
{
	unsigned f;
	f = islower(c) ? _lowerFlagTable[c - 'a'] : (c == 'O') * LOCAL_OPERATOR;
	return (static_cast<Flag>(f));
}

std::string UserMode::toString() const
{
	std::string s;
	if (isSet(INVISIBLE))
		s.push_back('i');
	if (isSet(WALLOPS))
		s.push_back('w');
	if (isSet(RESTRICTED))
		s.push_back('r');
	if (isSet(OPERATOR))
		s.push_back('o');
	if (isSet(LOCAL_OPERATOR))
		s.push_back('O');
	return (s);
}

const unsigned short UserMode::_lowerFlagTable[] =
{
	AWAY, 0, 0, 0, 0, 0, 0, 0, INVISIBLE, 0, 0, 0, 0, 0, OPERATOR, 0, 0,
	RESTRICTED, MARK, 0, 0, 0, WALLOPS, 0, 0, 0
};

User::User(TcpSocket *socket, UserRequirement::Flag requirements) :
BasicConnection(socket, USER),
_requirements(requirements),
_joinedChannels(0),
_idle(time(NULL))
{}

User::~User()
{}

bool User::isRegistered() const
{
	return (!_requirements.flags());
}

void User::mark()
{
	_umode.set(UserMode::MARK);
}

void User::unmark()
{
	_umode.unset(UserMode::MARK);
}

void User::unsetRequirement(UserRequirement::Flag requirement)
{
	_requirements.unset(requirement);
}

const UserRequirement &User::requirements() const
{
	return (_requirements);
}

const std::string &User::realname() const
{
	return (_realname);
}

time_t &User::idle()
{
	return (_idle);
}

const UserMode &User::umode() const
{
	return (_umode);
}

size_t User::joinedChannels() const
{
	return (_joinedChannels);
}

const std::string &User::awayReason() const
{
	return (_awayReason);
}

const std::string &User::prefix() const
{
	return (_prefix);
}

const std::string &User::nickname() const
{
	return (_nickname);
}

const std::string &User::username() const
{
	return (_username);
}

void User::setRealname(const std::string &realname)
{
	_realname = realname;
}

void User::setUmode(const UserMode &umode)
{
	_umode = umode;
}

void User::setJoinedChannels(size_t joinedChannels)
{
	_joinedChannels = joinedChannels;
}

void User::setAway(const std::string &reason)
{
	_awayReason = reason;
}

void User::setNickname(const std::string &nickname)
{
	_nickname = nickname;
	updatePrefix();
}

void User::setUsername(const std::string &username)
{
	_username = username;
	updatePrefix();
}

void User::updatePrefix()
{
	_prefix = nickname();
	if (_username.size())
		_prefix += "!" + username();
	_prefix += "@" + _socket->host();
}
