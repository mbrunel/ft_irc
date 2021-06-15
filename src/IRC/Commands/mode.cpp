#include "IrcServer.hpp"

enum SwitchFlag
{
	/** Unset switch */
	UNSET = -1,
	/** Set switch for user mode, else prints value associated with flag */
	NONE,
	/** Set switch */
	SET
};

static std::string parseModes(const IRC::Param &s, char *mask)
{
	std::string d;
	bool found[128] = {0};
	char maskValue = NONE;

	for (size_t i = 0; i < s.size(); ++i)
		if (s[i] == '-')
			maskValue = UNSET;
		else if (s[i] == '+')
			maskValue = SET;
		else if (s[i] > 0)
		{
			int j = s[i];
			mask[j] = maskValue;
			if (!found[j])
				d.push_back(s[i]);
			found[j] = 1;
		}
	return (d);
}

static bool parseNextModeParam(const std::vector<IRC::Param> &params, size_t &i, IRC::Param &dst)
{
	if (i >= params.size())
		return (0);
	dst = params[i++];
	return (1);
}

static bool extractMask(IRC::Param &arg)
{
	arg = arg.mask();
	size_t userPrefix = arg.find_first_of('!'), hostPrefix = arg.find_first_of('@');
	if (!userPrefix)
		arg = '*' + arg;
	if (hostPrefix != IRC::Param::npos)
		return (userPrefix == IRC::Param::npos || userPrefix < hostPrefix);
	arg += userPrefix == IRC::Param::npos ? "!*@*" : "@*";
	return (1);
}

static void pushChange(std::string &changes, const SwitchFlag prevSwitchFlag, const SwitchFlag switchFlag, char c)
{
	if (switchFlag != prevSwitchFlag)
		changes.push_back(switchFlag == UNSET ? '-' : '+');
	changes.push_back(c);
}

int IrcServer::mode(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().empty())
		return (writeNum(u, IRC::Error::needmoreparams(m.command())));
	const IRC::Param &s = m.params()[0];
	std::string modes, changes;
	char mask[128] = {0};
	SwitchFlag prevSwitchFlag = NONE, switchFlag;
	bool unknownMode = 0, needMoreParams = 0;
	if (s.isNickname())
	{
		if (!network.getByNickname(s))
			return (writeNum(u, IRC::Error::nosuchnick(s)));
		if (u.nickname() != s)
			return (writeNum(u, IRC::Error::usersdontmatch()));
		if (m.params().size() == 1)
			return (!writeNum(u, IRC::Reply::umodeis(u.umode().toString())));
		UserMode um(u.umode());
		modes = parseModes(m.params()[1], mask);
		for (size_t i = 0; i < modes.size(); ++i, prevSwitchFlag = switchFlag)
		{
			int modeC = modes[i];
			switchFlag = static_cast<SwitchFlag>(mask[modeC]);
			UserMode::Flag f = um.parse(modeC);
			if (f)
			{
				// User shouldn't be able to set this flag with MODE
				if (f == UserMode::AWAY)
					continue ;
				if (switchFlag == UNSET)
				{
					if (!um.isSet(f) || (!um.isSet(UserMode::OPERATOR | UserMode::LOCAL_OPERATOR)
					&& f == UserMode::RESTRICTED))
						continue ;
					um.unset(f);
				}
				else
				{
					if (um.isSet(f) || f == UserMode::OPERATOR || f == UserMode::LOCAL_OPERATOR)
						continue ;
					um.set(f);
				}
				pushChange(changes, prevSwitchFlag, switchFlag, modeC);
			}
			else
				unknownMode = 1;
		}
		u.setUmode(um);
		if (changes.size())
		{
			IRC::MessageBuilder r(u.nickname(), m.command());
			r << u.nickname() << changes;
			const std::string s = r.str();
			u.writeLine(s);
		}
	}
	else if (s.isChannel())
	{
		Channel *c = network.getByChannelname(s);
		if (!c)
			return (writeNum(u, IRC::Error::nosuchchannel(s)));
		if (c->type() == Channel::UNMODERATED)
			return (writeNum(u, IRC::Error::nochanmodes(s)));
		const MemberMode *mm = c->findMember(&u);
		if (m.params().size() == 1)
		{
			const ChannelMode &m = c->mode();
			IRC::MessageBuilder res(config.servername, IRC::Reply::channelmodeis(s, m.toString()), u.nickname());
			if (mm)
			{
				if (m.isSet(ChannelMode::KEY))
					res << c->key();
				if (m.isSet(ChannelMode::LIMIT))
				{
					std::stringstream ss;
					ss << c->limit();
					res << ss.str();
				}
			}
			u.writeLine(res.str());
			return (0);
		}
		if (!mm)
			return (writeNum(u, IRC::Error::notonchannel(s)));
		if (!mm->isSet(MemberMode::OPERATOR))
			return (writeNum(u, IRC::Error::chanoprisneeded(s)));
		std::list<std::string> changeParams;
		ChannelMode cm(c->mode());
		size_t argi = 2; // current index for modeparams
		IRC::Param arg;
		modes = parseModes(m.params()[1], mask);
		for (size_t i = 0; i < modes.size(); ++i, prevSwitchFlag = switchFlag)
		{
			int modeC = modes[i];
			switchFlag = static_cast<SwitchFlag>(mask[modeC]);
			ChannelMode::Flag f = cm.parse(modeC);
			if (f)
			{
				if (switchFlag == NONE) // Prints mode param
				{
					if (f < ChannelMode::BAN_MASK)
						continue ;
					if (f == ChannelMode::BAN_MASK)
					{
						const MaskSet &set = c->masks(Channel::BAN_SET);
						for (MaskSet::const_iterator i = set.begin(); i != set.end(); ++i)
							writeNum(u, IRC::Reply::banlist(c->name(), *i));
						writeNum(u, IRC::Reply::endofbanlist(c->name()));
					}
					else if (f == ChannelMode::EXCEPTION_MASK)
					{
						const MaskSet &set = c->masks(Channel::EXCEPTION_SET);
						for (MaskSet::const_iterator i = set.begin(); i != set.end(); ++i)
							writeNum(u, IRC::Reply::exceptlist(c->name(), *i));
						writeNum(u, IRC::Reply::endofexceptlist(c->name()));
					}
					else // if (f == ChannelMode::INVITATION_MASK)
					{
						const MaskSet &set = c->masks(Channel::INVITATION_SET);
						for (MaskSet::const_iterator i = set.begin(); i != set.end(); ++i)
							writeNum(u, IRC::Reply::invitelist(c->name(), *i));
						writeNum(u, IRC::Reply::endofinvitelist(c->name()));
					}
				}
				else
				{
					if (switchFlag == UNSET)
					{
						if (f > ChannelMode::LIMIT)
						{
							if (!parseNextModeParam(m.params(), argi, arg))
							{
								needMoreParams = 1;
								continue ;
							}
							else if (!extractMask(arg))
								continue ;
							Channel::MaskSetType type;
							if (f == ChannelMode::BAN_MASK)
								type = Channel::BAN_SET;
							else if (f == ChannelMode::EXCEPTION_MASK)
								type = Channel::EXCEPTION_SET;
							else // if (f == ChannelMode::INVITATION_MASK)
								type = Channel::INVITATION_SET;
							const MaskSet &set = c->masks(type);
							if (set.find(arg) == set.end())
								continue ;
							c->delMask(type, arg);
							changeParams.push_back(arg);
						}
						else if (!cm.isSet(f))
							continue ;
						cm.unset(f);
					}
					else
					{
						if (f >= ChannelMode::KEY)
						{
							if (!parseNextModeParam(m.params(), argi, arg))
							{
								needMoreParams = 1;
								continue ;
							}
							else if (f == ChannelMode::KEY)
							{
								if (cm.isSet(f))
								{
									writeNum(u, IRC::Error::keyset(s));
									continue ;
								}
								else if (!arg.isKey())
									continue ;
								c->setKey(arg);
								changeParams.push_back(arg);
							}
							else if (f == ChannelMode::LIMIT)
							{
								size_t n = strtoul(arg.c_str(), NULL, 10);
								c->setLimit(n);
								std::stringstream ss;
								ss << n;
								changeParams.push_back(ss.str());
							}
							else
							{
								if (!extractMask(arg))
									continue ;
								Channel::MaskSetType type;
								if (f == ChannelMode::BAN_MASK)
									type = Channel::BAN_SET;
								else if (f == ChannelMode::EXCEPTION_MASK)
									type = Channel::EXCEPTION_SET;
								else // if (f == ChannelMode::INVITATION_MASK)
									type = Channel::INVITATION_SET;
								const MaskSet &set = c->masks(type);
								if (set.size() == config.maxMasks || set.find(arg) != set.end())
									continue ;
								c->addMask(type, arg);
								changeParams.push_back(arg);
							}
						}
						else if (((f == ChannelMode::PRIVATE || f == ChannelMode::SECRET) && cm.isSet(ChannelMode::PRIVATE | ChannelMode::SECRET)) || cm.isSet(f))
							continue ;
						cm.set(f);
					}
					pushChange(changes, prevSwitchFlag, switchFlag, modeC);
				}
			}
			else if (!mm->parse(modeC))
				unknownMode = 1;
			else if (!parseNextModeParam(m.params(), argi, arg)) // MemberMode require target
				needMoreParams = 1;
			else if (switchFlag != NONE) // Disable implicit set switch
			{
				MemberMode::Flag f = mm->parse(modeC);
				if (!f)
				{
					unknownMode = 1;
					continue ;
				}
				User *user = network.getByNickname(arg);
				if (!user)
				{
					writeNum(u, IRC::Error::nosuchnick(arg));
					continue ;
				}
				MemberMode *umm = c->findMember(user);
				if (!umm)
				{
					writeNum(u, IRC::Error::usernotinchannel(arg, s));
					continue ;
				}
				if (switchFlag == UNSET)
				{
					if (!umm->isSet(f))
						continue ;
					umm->unset(f);
				}
				else
				{
					if (f == MemberMode::CREATOR || umm->isSet(f))
						continue ;
					umm->set(f);
				}
				pushChange(changes, prevSwitchFlag, switchFlag, modeC);
				changeParams.push_back(arg);
			}
		}
		c->setMode(cm);
		if (changes.size())
		{
			IRC::MessageBuilder r(u.nickname(), m.command());
			r << c->name() << changes;
			while (changeParams.size())
			{
				r << changeParams.front();
				changeParams.pop_front();
			}
			const std::string s = r.str();
			c->send(r.str());
		}
	}
	else
		return (writeNum(u, IRC::Error::nosuchnick(s)));
	if (needMoreParams)
		writeNum(u, IRC::Error::needmoreparams(m.command()));
	else if (unknownMode)
		writeNum(u, IRC::Error::umodeunknownflag());
	return (0);
}
