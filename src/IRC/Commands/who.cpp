#include "IrcServer.hpp"
#include "ft.hpp"

int     IrcServer::who(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));

	std::vector<Param> pMsg = m.params();
 
	 // J'ai limité par defaut, comme pour les serveurs existants
	if (!pMsg.size())
		return (writeNum(u, IrcError::needmoreparams(m.command())));

	bool ope = false;

	std::string mask = pMsg[0];

	if (pMsg.size() > 1)
		if (pMsg[1] == "o")
			ope = true;

	const Network::ChannelMap &channels = network.channels();
	Network::ChannelMap::const_iterator chanIter = channels.begin();
	while (chanIter != channels.end())
	{
		Channel *c = chanIter->second;
		ChannelMode cmode = c->mode();
		if ((cmode.isSet(ChannelMode::PRIVATE)
		|| cmode.isSet(ChannelMode::SECRET)) && !c->findMember(&u))
		{
			++chanIter;
			continue;
		}

		if (ft::match(mask, c->name()))
		{
			const Channel::MemberMap &members = c->members();
			Channel::MemberMap::const_iterator memberIter = members.begin();
			while (memberIter != members.end())
			{
				User *us = memberIter->first;
				MemberMode *mmode = c->findMember(us);
				UserMode umode = us->umode();

				if ((ope && (!umode.isSet(UserMode::OPERATOR)
				&& !mmode->isSet(UserMode::OPERATOR)))
				|| umode.isSet(UserMode::INVISIBLE))
				{
					++memberIter;
					continue;
				}

				// ---------------------------------------------- //


				std::string msg = "";

				msg += c->name() + " " + us->username();
				msg += " " + us->socket()->host();
				msg += " " + config.servername + " " + us->nickname();


				if (!umode.isSet(UserMode::AWAY))
					msg += " H";
				else
					msg += " G";
				if (umode.isSet(UserMode::OPERATOR))
					msg += "*";
				if (mmode->isSet(MemberMode::OPERATOR))
					msg += "@";



				msg += " :0 " + us->realname();

				// ---------------------------------------------- //

				writeNum(u, IrcReply::whoreply(msg));

				++memberIter;

			}
			writeNum(u, IrcReply::endofwho(mask));
			return 0;
		}
		++chanIter;
	}

	const Network::UserMap &users = network.users();
	Network::UserMap::const_iterator userIter = users.begin();
	while (userIter != users.end())
	{
		User *us = userIter->second;
		if (ft::match(mask, us->nickname())
		|| ft::match(mask, us->realname())
		|| ft::match(mask, us->username()))
		{
			// ---------------------------------------------- //

			UserMode umode = us->umode();

			if ((ope && !umode.isSet(UserMode::OPERATOR))
			|| umode.isSet(UserMode::INVISIBLE))
			{
				++userIter;
				continue;
			}


			std::string msg;

			msg += "* " + us->username();
			msg += " " + us->socket()->host();
			msg += " " + config.servername + " " + us->nickname();

			if (!umode.isSet(UserMode::AWAY))
				msg += " H";
			else
				msg += " G";
			if (umode.isSet(UserMode::OPERATOR))
				msg += "*";

			msg += " :0 " + us->realname();

			// ---------------------------------------------- //

			writeNum(u, IrcReply::whoreply(msg));
		}
		++userIter;
	}

	writeNum(u, IrcReply::endofwho(mask));
	return (0);
}

/*
Command : WHO
Params : [<mask> [+ "o"]]
*/
