#include <cctype>
#include "Channel.hpp"
#include "Utils.hpp"

MemberMode::MemberMode(unsigned flags) :
Mode(flags)
{}

MemberMode::~MemberMode()
{}

MemberMode::Flag MemberMode::parse(char c)
{
	if (c == 'O')
		return (CREATOR);
	if (c == 'o')
		return (OPERATOR);
	return (static_cast<Flag>((c == 'v') * VOICE));
}

ChannelMode::ChannelMode(unsigned flags) :
Mode(flags)
{}

ChannelMode::~ChannelMode()
{}

ChannelMode::Flag ChannelMode::parse(char c)
{
	unsigned f;
	f = islower(c) ? _lowerFlagTable[c - 'a'] : (c == 'I') * INVITATION_MASK;
	return (static_cast<Flag>(f));
}

std::string ChannelMode::toString() const
{
	std::string s;
	if (isSet(ANONYMOUS))
		s.push_back('a');
	if (isSet(INVITE_ONLY))
		s.push_back('i');
	if (isSet(NO_OUTSIDE_MSG))
		s.push_back('n');
	if (isSet(QUIET))
		s.push_back('q');
	if (isSet(PRIVATE))
		s.push_back('p');
	if (isSet(SECRET))
		s.push_back('s');
	if (isSet(REOP))
		s.push_back('r');
	if (isSet(TOPIC_SETTABLE_BY_CHANOP))
		s.push_back('t');
	if (isSet(KEY))
		s.push_back('k');
	if (isSet(LIMIT))
		s.push_back('l');
	return (s);
}

const unsigned short ChannelMode::_lowerFlagTable[] =
{
	ANONYMOUS, BAN_MASK, 0, 0, EXCEPTION_MASK, 0, 0, 0, INVITE_ONLY, 0, KEY,
	LIMIT, MODERATED, NO_OUTSIDE_MSG, 0, PRIVATE, QUIET, REOP, SECRET,
	TOPIC_SETTABLE_BY_CHANOP, 0, 0, 0, 0, 0, 0
};

Channel::Channel(const std::string &name) :
_name(name), _type(GLOBAL)
{
	const char *types = "#&+";
	if (name.size())
		_type = static_cast<Type>(strchrnul(types, name[0]) - types);
}

Channel::~Channel()
{}

void Channel::addMember(User *user, const MemberMode &mode)
{
	_members[user] = mode;
	user->setJoinedChannels(user->joinedChannels() + 1);
	MaskSet::const_iterator it = _invitations.find(user->nickname());
	if (it != _invitations.end())
		_invitations.erase(it);
}

void Channel::banMember(User *user)
{
	if (_type != UNMODERATED)
		_bans.insert(user->nickname());
	delMember(user);
}

size_t Channel::count() const
{
	return (_members.size());
}

void Channel::delMember(User *user)
{
	_members.erase(user);
	user->setJoinedChannels(user->joinedChannels() - 1);
}

MemberMode *Channel::findMember(User *user)
{
	MemberMap::iterator i(_members.find(user));
	return (i != _members.end() ? &i->second : NULL);
}

void Channel::invite(User *user)
{
	_invitations.insert(user->nickname());
}

bool Channel::isBanned(const User *u) const
{
	return (_bans.find(u->nickname()) != _bans.end() || (inSet(u->nickname(), _banMasks) && !inSet(u->nickname(), _exceptionMasks)));
}

bool Channel::isInvited(const User *u) const
{
	return (_invitations.find(u->nickname()) != _invitations.end() || inSet(u->nickname(), _invitationMasks));
}

bool Channel::isLocal() const
{
	return (_type == LOCAL);
}

void Channel::markAllMembers()
{
	for (MemberMap::const_iterator i = _members.begin(); i != _members.end(); ++i)
		i->first->mark();
}

void Channel::send(const std::string &msg, BasicConnection *origin, bool useReceipt) const
{
	for (MemberMap::const_iterator i = _members.begin(); i != _members.end(); ++i)
	{
		User *user(i->first);
		if (!user->hopcount() && (!origin || user->socket() != origin->socket()) &&
		(!useReceipt || !user->umode().isSet(UserMode::MARK)))
			user->writeLine(msg);
	}
}

unsigned int Channel::nbUserVisible() const
{
	unsigned int nb = 0;
	MemberMap::const_iterator im = _members.begin();

	while (im != _members.end())
	{
		const UserMode &umode = im->first->umode();
		if (!umode.isSet(UserMode::INVISIBLE))
				nb++;
		++im;
	}
	return (nb);
}

const std::string &Channel::name() const
{
	return (_name);
}

Channel::Type Channel::type() const
{
	return (_type);
}

const Channel::MemberMap &Channel::members() const
{
	return (_members);
}

const ChannelMode &Channel::mode() const
{
	return (_mode);
}

const std::string &Channel::topic() const
{
	return (_topic);
}

const std::string &Channel::key() const
{
	return (_key);
}

size_t Channel::limit() const
{
	return (_limit);
}

Channel::MaskSet &Channel::banMasks()
{
	return (_banMasks);
}

Channel::MaskSet &Channel::exceptionMasks()
{
	return (_exceptionMasks);
}

Channel::MaskSet &Channel::invitationMasks()
{
	return (_invitationMasks);
}

void Channel::setMode(const ChannelMode &mode)
{
	_mode = mode;
}

void Channel::setTopic(const std::string &topic)
{
	_topic = topic;
}

void Channel::setKey(const std::string &key)
{
	_key = key;
}

void Channel::setLimit(size_t limit)
{
	_limit = limit;
}

bool Channel::inSet(const std::string &nickname, const Channel::MaskSet &set) const
{
	for (Channel::MaskSet::const_iterator it = set.begin(); it != set.end(); ++it)
		if (Utils::match(*it, nickname))
			return (1);
	return (0);
}
