/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 11:48:50 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/20 17:21:26 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

static void parseModes(const Param &s, std::string &modes, bool *mask)
{
	bool found[128] = {0};
	bool set = 1;

	for (size_t i = 0; i < s.size(); ++i)
		if (s[i] == '-')
			set = 0;
		else if (s[i] == '+')
			set = 1;
		else if (s[1] > 0)
		{
			mask[(int)s[i]] = set;
			if (!found[(int)s[i]])
			{
				found[(int)s[i]] = 1;
				modes.push_back(s[i]);
			}
		}
}

static void confirm(Network &n, User &u, char sign, char fv)
{
	std::string buf("??"); // aled
	MessageBuilder r(u.nickname(), "MODE");
	buf[0] = sign;
	buf[1] = fv;
	r << u.nickname() << buf;
	const std::string s = r.str();
	u.writeLine(s);
	n.msgToNetwork(s, &u);
}

static void set(Network &n, User &u, UserMode &um, UserMode::Flag f, char fv)
{
	if (um.isSet(f))
		return ;
	confirm(n, u, '+', fv);
	um.set(f);
}

static void unset(Network &n, User &u, UserMode &um, UserMode::Flag f, char fv)
{
	if (!um.isSet(f))
		return ;
	confirm(n, u, '-', fv);
	um.unset(f);
}

int IrcServer::mode(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().empty())
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	const Param &s = m.params()[0];
	std::string modes;
	bool mask[128] = {0};
	bool unknownMode = 0;

	if (s.isNickname())
	{
		User *t = network.getByNickname(s);
		if (!t)
			return (writeNum(u, IrcError::nosuchnick(s)));
		if (m.params().size() > 1)
		{
			if (u.nickname() != s)
				return (writeNum(u, IrcError::usersdontmatch()));
			UserMode um(u.umode());
			parseModes(m.params()[1], modes, mask);
			for (size_t i = 0; i < modes.size(); ++i)
			{
				if (modes[i] == 'i')
				{
					if (mask[(int)modes[i]])
						set(network, u, um, UserMode::INVISIBLE, 'i');
					else
						unset(network, u, um, UserMode::INVISIBLE, 'i');
				}
				else if (modes[i] == 'w')
				{
					if (mask[(int)modes[i]])
						set(network, u, um, UserMode::WALLOPS, 'w');
					else
						unset(network, u, um, UserMode::WALLOPS, 'w');
				}
				else if (modes[i] == 'r')
				{
					if (mask[(int)modes[i]])
						set(network, u, um, UserMode::RESTRICTED, 'r');
					else
						unset(network, u, um, UserMode::RESTRICTED, 'r');
				}
				else if (modes[i] == 'o' && !mask[(int)modes[i]])
					unset(network, u, um, UserMode::OPERATOR, 'o');
				else if (modes[i] == 'O' && !mask[(int)modes[i]])
					unset(network, u, um, UserMode::LOCAL_OPERATOR, 'O');
				else if (!strchr("aroOs", modes[i]) && !unknownMode)
				{
					writeNum(u, IrcError::umodeunknownflag());
					unknownMode = 1;
				}
			}
			u.setUmode(um);
		}
		else
		{
			std::string s;
			const UserMode &um = t->umode();
			if (um.isSet(UserMode::INVISIBLE))
				s.push_back('i');
			if (um.isSet(UserMode::WALLOPS))
				s.push_back('w');
			if (um.isSet(UserMode::RESTRICTED))
				s.push_back('r');
			if (um.isSet(UserMode::OPERATOR))
				s.push_back('o');
			if (um.isSet(UserMode::LOCAL_OPERATOR))
				s.push_back('O');
			writeNum(u, IrcReply::umodeis(s));
		}
	}
	else if (s.isChannel())
	{
		if (m.params().size())
		{
			if (u.nickname() != s)
				return (writeNum(u, IrcError::usersdontmatch()));
			parseModes(m.params()[1], modes, mask);
		}
		else
			; // Prints channel modes
	}
	else
		return (writeNum(u, IrcError::nosuchnick(s)));
	return (0);
}
