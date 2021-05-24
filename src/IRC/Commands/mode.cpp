/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 11:48:50 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/24 15:52:11 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static bool parseNextModeParam(const std::vector<Param> &params, size_t &i, std::string &dst)
{
	if (i >= params.size())
		return (0);
	dst = params[i++];
	return (1);
}

static void pushChange(std::string &changes, SwitchFlag &prevSwitchFlag, SwitchFlag switchFlag, char c)
{
	if (switchFlag != prevSwitchFlag)
		changes.push_back(switchFlag == UNSET ? '-' : '+');
	changes.push_back(c);
	prevSwitchFlag = switchFlag;
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
	SwitchFlag prevSwitchFlag = static_cast<SwitchFlag>(-1);
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
		for (size_t i = 0; i < modes.size(); ++i)
		{
			int modeC = modes[i];
			SwitchFlag switchFlag = static_cast<SwitchFlag>(mask[modeC]);
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
			network.msgToNetwork(s, &u);
		}
	}
	else if (s.isChannel())
	{
		Channel *c = network.getByChannelname(s);
		if (!c)
			return (writeNum(u, IrcError::nosuchchannel(s)));
		if (c->type() == Channel::UNMODERATED)
			return (writeNum(u, IrcError::nochanmodes(s)));
		if (m.params().size() == 1)
			return (!writeNum(u, IrcReply::channelmodeis(s, c->mode().toString())));
		const MemberMode *mm = c->findMember(&u);
		if (!mm)
			return (writeNum(u, IrcError::notonchannel(s)));
		if (!mm->isSet(MemberMode::OPERATOR))
			return (writeNum(u, IrcError::chanoprisneeded(s)));
		std::list<std::string> changeParams;
		ChannelMode cm(c->mode());
		size_t argi = 2; // current index for modeparams
		std::string arg;
		modes = parseModes(m.params()[1], mask);
		for (size_t i = 0; i < modes.size(); ++i)
		{
			int modeC = modes[i];
			SwitchFlag switchFlag = static_cast<SwitchFlag>(mask[modeC]);
			ChannelMode::Flag f = cm.parse(modeC);
			if (f)
			{
				if (switchFlag == NONE) // Prints mode param
				{
					if (f < ChannelMode::BAN_MASK)
						continue ;
					if (f == ChannelMode::BAN_MASK)
					{
						// Prints bans list
					}
					else if (f == ChannelMode::EXCEPTION_MASK)
					{
						// Prints exceptions list
					}
					else
					{
						// Prints invitations list
					}
				}
				else
				{
					if (f >= ChannelMode::KEY && !parseNextModeParam(m.params(), argi, arg))
						needMoreParams = 1;
					else if (switchFlag == UNSET)
					{
						if (!cm.isSet(f))
							continue ;
						cm.unset(f);
					}
					else
					{
						if (cm.isSet(f))
						{
							if (f == ChannelMode::KEY)
								writeNum(u, IrcError::keyset(s));
							continue ;
						}
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
					if (!cm.isSet(f))
						continue ;
					umm->unset(f);
				}
				else
				{
					if (cm.isSet(f))
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
			r << u.nickname() << changes;
			while (changeParams.size())
			{
				r << changeParams.front();
				changeParams.pop_front();
			}
			const std::string s = r.str();
			u.writeLine(s);
			network.msgToChan(c, r.str());
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
