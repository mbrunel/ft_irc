/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumeric.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:37:07 by mbrunel           #+#    #+#             */
/*   Updated: 2021/05/24 19:27:01 by mapapin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"

IrcNumeric::IrcNumeric(const IrcNumericCode code, const std::string &content) :
code(code), content(content)
{}

namespace IrcError
{
	const IrcNumeric nosuchnick(const std::string &nickname)
	{
		return (IrcNumeric(ERR_NOSUCHNICK, nickname + " :No such nick/channel"));
	}

	const IrcNumeric nosuchserver(const std::string &servername)
	{
		return (IrcNumeric(ERR_NOSUCHSERVER, servername + " :No such server"));
	}

	const IrcNumeric nosuchchannel(const std::string &channelname)
	{
		return (IrcNumeric(ERR_NOSUCHCHANNEL, channelname + " :No such channel"));
	}

	const IrcNumeric cannotsendtochan(const std::string &channelname)
	{
		return (IrcNumeric(ERR_CANNOTSENDTOCHAN, channelname + " :Cannot send to channel"));
	}

	const IrcNumeric toomanychannels(const std::string &channelname)
	{
		return (IrcNumeric(ERR_TOOMANYCHANNELS, channelname + " :You have joined too many channels"));
	}

	const IrcNumeric wasnosuchnick(const std::string &nickname)
	{
		return (IrcNumeric(ERR_WASNOSUCHNICK, nickname + " :There was no such nickname"));
	}

	const IrcNumeric toomanytargets(const std::string &target)
	{
		return (IrcNumeric(ERR_TOOMANYTARGETS, target + " :Duplicate recipients. No message delivered"));
	}

	const IrcNumeric noorigin()
	{
		return (IrcNumeric(ERR_NOORIGIN, ":No origin specified"));
	}

	const IrcNumeric norecipient(const std::string &command)
	{
		return (IrcNumeric(ERR_NORECIPIENT, ":No recipient given " + command));
	}

	const IrcNumeric notexttosend()
	{
		return (IrcNumeric(ERR_NOTEXTTOSEND, ":No text to send"));
	}

	const IrcNumeric notoplevel(const std::string &mask)
	{
		return (IrcNumeric(ERR_NOTOPLEVEL, mask + " :No top level domain specified"));
	}

	const IrcNumeric wildtoplevel(const std::string &mask)
	{
		return (IrcNumeric(ERR_WILDTOPLEVEL, mask + " :Wildcard in top level domain"));
	}

	const IrcNumeric unknowncommand(const std::string &command)
	{
		return (IrcNumeric(ERR_UNKNOWNCOMMAND, command + " :Unknown command"));
	}

	const IrcNumeric nomotd()
	{
		return (IrcNumeric(ERR_NOMOTD, ":MOTD file is missing"));
	}

	const IrcNumeric noadmininfo()
	{
		return (IrcNumeric(ERR_NOADMINFO, ":No administrative info available"));
	}

	const IrcNumeric fillerror(const std::string &file_op, const std::string &file)
	{
		return (IrcNumeric(ERR_FILERROR, ":File error doing " + file_op + " : on " + file));
	}

	const IrcNumeric nonicknamegiven()
	{
		return (IrcNumeric(ERR_NONICKNAMEGIVEN, ":No nickame given"));
	}

	const IrcNumeric erroneusnickname(const std::string &nickname)
	{
		return (IrcNumeric(ERR_ERRONEUSNICKNAME, nickname + " :Erroneus nickname"));
	}

	const IrcNumeric nicknameinuse(const std::string &nickname)
	{
		return (IrcNumeric(ERR_NICKNAMEINUSE, nickname + " :Nickname is already in use"));
	}

	const IrcNumeric nickcollision(const std::string &nickname)
	{
		return (IrcNumeric(ERR_NICKCOLLISION, nickname + " :Nickname collision KILL"));
	}

	const IrcNumeric usernotinchannel(const std::string &nickname, const std::string &channel)
	{
		return (IrcNumeric(ERR_USERNOTINCHANNEL, nickname + ' ' + channel + " :They aren't on that channel"));
	}

	const IrcNumeric notonchannel(const std::string &channel)
	{
		return (IrcNumeric(ERR_NOTONCHANNEL, channel + " :You're not on that channel"));
	}

	const IrcNumeric useronchannel(const std::string &nickname, const std::string &channel)
	{
		return (IrcNumeric(ERR_USERONCHANNEL, nickname + ' ' + channel + " :is already on channel"));
	}

	const IrcNumeric nologin(const std::string &user)
	{
		return (IrcNumeric(ERR_NOLOGIN, user + " :User not logged in"));
	}

	const IrcNumeric summondisabled()
	{
		return (IrcNumeric(ERR_SUMMONDISABLED, ":Summon has been disabled"));
	}

	const IrcNumeric usersdisabled()
	{
		return (IrcNumeric(ERR_USERDISABLED, ":USERS has been disabled"));
	}

	const IrcNumeric notregistered()
	{
		return (IrcNumeric(ERR_NOTREGISTERED, ":You have not registered"));
	}

	const IrcNumeric needmoreparams(const std::string &command)
	{
		return (IrcNumeric(ERR_NEEDMOREPARAMS, command + " :Not enough parameters"));
	}

	const IrcNumeric alreadyregistred()
	{
		return (IrcNumeric(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)"));
	}

	const IrcNumeric nopermforhost()
	{
		return (IrcNumeric(ERR_NOPERMFORHOST, ":Your host is not among the privileged"));
	}

	const IrcNumeric passwdmissmatch()
	{
		return (IrcNumeric(ERR_PASSWDMISSMATCH, ":Password incorrect"));
	}

	const IrcNumeric yourebannedcreep()
	{
		return (IrcNumeric(ERR_YOUREBANNEDCREEP, ":You are banned from this server"));
	}

	const IrcNumeric keyset(const std::string &channel)
	{
		return (IrcNumeric(ERR_KEYSET, channel + " :Channel key already set"));
	}

	const IrcNumeric channelisfull(const std::string &channel)
	{
		return (IrcNumeric(ERR_CHANNELISFULL, channel + " :Channel is full"));
	}

	const IrcNumeric unknownmode(const std::string &c)
	{
		return (IrcNumeric(ERR_UNKNOWNMODE, c + " :is unknown mode char to me"));
	}

	const IrcNumeric inviteonlychan(const std::string &channel)
	{
		return (IrcNumeric(ERR_INVITEONLYCHAN, channel + " :Cannot join channel (+i)"));
	}

	const IrcNumeric bannedfromchan(const std::string &channel)
	{
		return (IrcNumeric(ERR_BANNEDFROMCHAN, channel + " :Cannot join channel (+b)"));
	}

	const IrcNumeric badchannelkey(const std::string &channel)
	{
		return (IrcNumeric(ERR_BADCHANNELKEY, channel + " :Cannot join channel (+k)"));
	}

	const IrcNumeric nochanmodes(const std::string &channel)
	{
		return (IrcNumeric(ERR_NOCHANMODES, channel + " :Channel doesn't support modes"));
	}

	const IrcNumeric noprivileges()
	{
		return (IrcNumeric(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator"));
	}

	const IrcNumeric chanoprisneeded(const std::string &channel)
	{
		return (IrcNumeric(ERR_CHANOPRIVISNEED, channel + " :You're not channel operator"));
	}

	const IrcNumeric cantkillserver()
	{
		return (IrcNumeric(ERR_CANTKILLSERVER, ":You can't kill a server"));
	}

	const IrcNumeric nooperhost()
	{
		return (IrcNumeric(ERR_NOPERMFORHOST, ":No O-lines for your host"));
	}

	const IrcNumeric umodeunknownflag()
	{
		return (IrcNumeric(ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag"));
	}

	const IrcNumeric usersdontmatch()
	{
		return (IrcNumeric(ERR_USERSDONTMATCH, ":Can't change mode for other users"));
	}
}


namespace IrcReply
{
	const IrcNumeric welcome(const std::string &userPrefix)
	{
		return (IrcNumeric(RPL_WELCOME, ":Welcome to the Internet Relay Network " + userPrefix));
	}

	const IrcNumeric yourhost(const std::string &servername, const std::string &version)
	{
		return (IrcNumeric(RPL_YOURHOST, ":Your host is " + servername + ", running version " + version));
	}

	const IrcNumeric created(const std::string &date)
	{
		return (IrcNumeric(RPL_CREATED, ":This server was created " + date));
	}

	const IrcNumeric myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes)
	{
		return (IrcNumeric(RPL_MYINFO, ':' + servername + ' ' + version + ' ' + availableUserModes + ' ' + availableChannelModes));
	}

	const IrcNumeric bounce(const std::string &servername, const std::string &port)
	{
		return (IrcNumeric(RPL_BOUNCE, ":Try server " + servername + ", port " + port));
	}

	const IrcNumeric umodeis(const std::string &modes)
	{
		return (IrcNumeric(RPL_UMODEIS, '+' + modes));
	}

	const IrcNumeric away(const std::string &nickname, const std::string &reason)
	{
		return (IrcNumeric(RPL_AWAY, nickname + " :" + reason));
	}

	const IrcNumeric unaway()
	{
		return (IrcNumeric(RPL_UNAWAY, ":You are no longer marked as being away"));
	}

	const IrcNumeric nowaway()
	{
		return (IrcNumeric(RPL_AWAY, ":You have been marked as being away"));
	}

	const IrcNumeric channelmodeis(const std::string &channel, const std::string &modes)
	{
		return (IrcNumeric(RPL_CHANNELMODEIS, channel + " +" + modes));
	}

	const IrcNumeric motd(const std::string &text)
	{
		return (IrcNumeric(RPL_MOTD, ":- " + text));
	}

	const IrcNumeric motdstart(const std::string &servername)
	{
		return (IrcNumeric(RPL_MOTDSTART, ":- " + servername + " Message of the day - "));
	}

	const IrcNumeric endofmotd()
	{
		return (IrcNumeric(RPL_ENDOFMOTD, ":End of MOTD command"));
	}

	const IrcNumeric youreoper()
	{
		return (IrcNumeric(RPL_YOUREOPER, "You are now an IRC operator"));
	}
	const IrcNumeric time()
	{
		char stamp[80];

		time_t rawtime;
		::time(&rawtime);
		strftime(stamp, 80, "%x - %I:%M:%S", localtime(&rawtime));
		return (IrcNumeric(RPL_TIME, stamp));
	}
}
