#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

enum SwitchFlag
{
	/** Unset switch */
	UNSET = -1,
	/** Set switch for user mode, else prints value associated with flag */
	NONE,
	/** Set switch */
	SET
};

static std::string parseModes(const Param &s, char *mask)
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

static bool parseNextModeParam(const std::vector<Param> &params, size_t &i, Param &dst)
{
	if (i >= params.size())
		return (0);
	dst = params[i++];
	return (1);
}

static void pushChange(std::string &changes, const SwitchFlag prevSwitchFlag, const SwitchFlag switchFlag, char c)
{
	if (switchFlag != prevSwitchFlag)
		changes.push_back(switchFlag == UNSET ? '-' : '+');
	changes.push_back(c);
}

int IrcServer::mode(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().empty())
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	const Param &s = m.params()[0];
	std::string modes, changes;
	char mask[128] = {0};
	SwitchFlag prevSwitchFlag = NONE, switchFlag;
	bool unknownMode = 0, needMoreParams = 0;
	if (s.isNickname())
	{
		if (!network.getByNickname(s))
			return (writeNum(u, IrcError::nosuchnick(s)));
		if (u.nickname() != s)
			return (writeNum(u, IrcError::usersdontmatch()));
		if (m.params().size() == 1)
			return (!writeNum(u, IrcReply::umodeis(u.umode().toString())));
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
			MessageBuilder r(u.nickname(), m.command());
			r << u.nickname() << changes;
			const std::string s = r.str();
			u.writeLine(s);
		}
	}
	else if (s.isChannel())
	{
		Channel *c = network.getByChannelname(s);
		if (!c)
			return (writeNum(u, IrcError::nosuchchannel(s)));
		if (c->type() == Channel::UNMODERATED)
			return (writeNum(u, IrcError::nochanmodes(s)));
		const MemberMode *mm = c->findMember(&u);
		if (m.params().size() == 1)
		{
			const ChannelMode &m = c->mode();
			MessageBuilder res(config.servername, IrcReply::channelmodeis(s, m.toString()), u.nickname());
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
			return (writeNum(u, IrcError::notonchannel(s)));
		if (!mm->isSet(MemberMode::OPERATOR))
			return (writeNum(u, IrcError::chanoprisneeded(s)));
		std::list<std::string> changeParams;
		ChannelMode cm(c->mode());
		size_t argi = 2; // current index for modeparams
		Param arg;
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
						const Channel::MaskSet &set = c->banMasks();
						for (Channel::MaskSet::const_iterator i = set.begin();
						i != set.end(); ++i)
							writeNum(u, IrcReply::banlist(c->name(), *i));
						writeNum(u, IrcReply::endofbanlist(c->name()));
					}
					else if (f == ChannelMode::EXCEPTION_MASK)
					{
						const Channel::MaskSet &set = c->exceptionMasks();
						for (Channel::MaskSet::const_iterator i = set.begin();
						i != set.end(); ++i)
							writeNum(u, IrcReply::exceptlist(c->name(), *i));
						writeNum(u, IrcReply::endofexceptlist(c->name()));
					}
					else
					{
						const Channel::MaskSet &set = c->invitationMasks();
						for (Channel::MaskSet::const_iterator i = set.begin();
						i != set.end(); ++i)
							writeNum(u, IrcReply::invitelist(c->name(), *i));
						writeNum(u, IrcReply::endofinvitelist(c->name()));
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
							else if (!arg.isMask())
								continue ;
							const std::string mask = arg.mask();
							Channel::MaskSet *set;
							if (f == ChannelMode::BAN_MASK)
								set = &c->banMasks();
							else if (f == ChannelMode::EXCEPTION_MASK)
								set = &c->exceptionMasks();
							else
								set = &c->invitationMasks();
							if (set->find(mask) == set->end())
								continue ;
							set->erase(mask);
							changeParams.push_back(mask);
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
									writeNum(u, IrcError::keyset(s));
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
								if (!arg.isMask())
									continue ;
								const std::string mask = arg.mask();
								Channel::MaskSet *set;
								if (f == ChannelMode::BAN_MASK)
									set = &c->banMasks();
								else if (f == ChannelMode::EXCEPTION_MASK)
									set = &c->exceptionMasks();
								else
									set = &c->invitationMasks();
								if (set->find(mask) != set->end())
									continue ;
								if (set->size() == config.maxMasks)
									continue ;
								set->insert(mask);
								changeParams.push_back(mask);
							}
						}
						else if (((f == ChannelMode::PRIVATE || f == ChannelMode::SECRET) && cm.isSet(ChannelMode::PRIVATE | ChannelMode::SECRET)) || cm.isSet(f))
							continue ;
						cm.set(f);
					}
					pushChange(changes, prevSwitchFlag, switchFlag, modeC);
				}
			}
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
					writeNum(u, IrcError::nosuchnick(arg));
					continue ;
				}
				MemberMode *umm = c->findMember(user);
				if (!umm)
				{
					writeNum(u, IrcError::usernotinchannel(arg, s));
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
			MessageBuilder r(u.nickname(), m.command());
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
		return (writeNum(u, IrcError::nosuchnick(s)));
	if (needMoreParams)
		writeNum(u, IrcError::needmoreparams(m.command()));
	else if (unknownMode)
		writeNum(u, IrcError::umodeunknownflag());
	return (0);
}
