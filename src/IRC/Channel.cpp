#include <cctype>
#include "Channel.hpp"
#include "Utils.hpp"

MemberMode::MemberMode(unsigned flags) :
Mode(flags)
{}

MemberMode::~MemberMode()
{}

bool MemberMode::canTalk() const
{
	return (isSet(OPERATOR | VOICE));
}

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
	if (isSet(INVITE_ONLY))
		s.push_back('i');
	if (isSet(NO_OUTSIDE_MSG))
		s.push_back('n');
	if (isSet(PRIVATE))
		s.push_back('p');
	if (isSet(SECRET))
		s.push_back('s');
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
	0, BAN_MASK, 0, 0, EXCEPTION_MASK, 0, 0, 0, INVITE_ONLY, 0, KEY,
	LIMIT, MODERATED, NO_OUTSIDE_MSG, 0, PRIVATE, 0, 0, SECRET,
	TOPIC_SETTABLE_BY_CHANOP, 0, 0, 0, 0, 0, 0
};

MaskSet::MaskSet()
{}

MaskSet::~MaskSet()
{}

bool MaskSet::inSet(const User &user) const
{
	for (const_iterator i = begin(); i != end(); ++i)
		if (Utils::match(*i, user.prefix()))
			return (1);
	return (0);
}

Channel::Channel(const std::string &name) :
_name(name), _type(name.c_str()[0] == '!' ? UNMODERATED : GLOBAL)
{}

Channel::~Channel()
{}

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

const MaskSet &Channel::masks(const MaskSetType type) const
{
	return (_masks[type]);
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

void Channel::addMember(User *user, const MemberMode &mode)
{
	_members[user] = mode;
	user->setJoinedChannels(user->joinedChannels() + 1);
	MaskSet::const_iterator it = _invitations.find(user->nickname());
	if (it != _invitations.end())
		_invitations.erase(it);
}

void Channel::addMask(const MaskSetType type, const std::string &mask)
{
	_masks[type].insert(mask);
}

bool Channel::canSendToChannel(User *user)
{
	MemberMode *um = findMember(user);
	if (um)
		return (!_mode.isSet(ChannelMode::MODERATED) || um->canTalk());
	return (!_mode.isSet(ChannelMode::MODERATED | ChannelMode::NO_OUTSIDE_MSG) && !isBanned(user));
}

void Channel::delMember(User *user)
{
	_members.erase(user);
	user->setJoinedChannels(user->joinedChannels() - 1);
}

void Channel::delMask(const MaskSetType type, const std::string &mask)
{
	_masks[type].erase(mask);
}

MemberMode *Channel::findMember(User *user)
{
	MemberMap::iterator i(_members.find(user));
	return (i != _members.end() ? &i->second : NULL);
}

void Channel::invite(const User *user)
{
	_invitations.insert(user->nickname());
}

bool Channel::isBanned(const User *u) const
{
	return (masks(BAN_SET).inSet(*u) && !masks(EXCEPTION_SET).inSet(*u));
}

bool Channel::isInvited(const User *u) const
{
	return (_invitations.find(u->nickname()) != _invitations.end() || masks(INVITATION_SET).inSet(*u));
}

void Channel::markAllMembers()
{
	for (MemberMap::const_iterator i = _members.begin(); i != _members.end(); ++i)
		i->first->mark();
}

size_t Channel::count() const
{
	return (_members.size());
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

size_t Channel::nbUserVisible() const
{
	size_t nb = 0;
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
