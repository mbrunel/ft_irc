/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/20 11:48:50 by asoursou          #+#    #+#             */
/*   Updated: 2021/05/20 15:29:56 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"
#include "MessageBuilder.hpp"

static void parseModes(const Param &s, std::string &modes, char *mask)
{
	bool found[128] = {0};
	bool set = 1;

	for (size_t i = 0; i < s.size(); ++i)
		if (s[i] == '-')
			set = -1;
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

int IrcServer::mode(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().empty())
		return (writeNum(u, IrcError::needmoreparams(m.command())));
	const Param &s = m.params()[0];
	std::string modes, buf("xx");
	char mask[128] = {0};
	bool unknownMode = 0;

	if (s.isNickname())
	{
		if (!network.getByNickname(s))
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
					if (mask[(int)modes[i]] < 0 && um.isSet(UserMode::INVISIBLE))
					{
						um.unset(UserMode::INVISIBLE);
						MessageBuilder r(u.nickname(), m.command());
						buf[0] = '-';
						buf[1] = modes[i];
						r << u.nickname() << buf;
						const std::string s = r.str();
						u.writeLine(s);
						network.msgToNetwork(s, &u);
					}
					else if (mask[(int)modes[i]] > 0 && !um.isSet(UserMode::INVISIBLE))
					{
						um.set(UserMode::INVISIBLE);
						MessageBuilder r(u.nickname(), m.command());
						buf[0] = '+';
						buf[1] = modes[i];
						r << u.nickname() << buf;
						const std::string s = r.str();
						u.writeLine(s);
						network.msgToNetwork(s, &u);
					}
				}
				else if (!strchr("aroOs", modes[i]) && !unknownMode)
				{
					writeNum(u, IrcError::umodeunknownflag());
					unknownMode = 1;
				}
			}
			u.setUmode(um);
		}
		else
			; // Prints user modes
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
	for (size_t i = 0; i < modes.size(); ++i)
	{
		if (mask[(int)modes[i]])
			std::cerr << (mask[(int)modes[i]] > 0 ? '+' : '-') << modes[i];
	}
	std::cerr << std::endl;
	return (0);
}
