/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumeric.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:25:14 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 17:22:35 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

enum IrcNumeric
{
	RPL_WELCOME = 1,
	RPL_YOURHOST = 2,
	RPL_CREATED = 3,
	RPL_MYINFO = 4,
	RPL_MOTD = 372,
	RPL_MOTDSTART = 375,
	RPL_ENDOFMOTD = 376,

	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHSERVER = 402,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNELS = 405,
	ERR_WASNOSUCHNICK = 406,
	ERR_TOOMANYTARGETS = 407,
	ERR_NOSCUCHSERVICE = 408,
	ERR_NOORIGIN = 409,
	ERR_NORECIPIENT = 411,
	ERR_NOTEXTTOSEND = 412,
	ERR_NOTOPLEVEL = 413,
	ERR_WILDTOPLEVEL = 414,
	ERR_BADMASK = 415,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NOMOTD = 422,
	ERR_NOADMINFO = 423,
	ERR_FILERROR = 424,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,
	ERR_NICKCOLLISION = 436,
	ERR_UNAVAILABLE_RESOURCE = 437,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOLOGIN = 444,
	ERR_SUMMONDISABLED = 445,
	ERR_USERDISABLED = 446,
	ERR_NOTREGISTERED = 447,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_NOPERMFORHOST = 463,
	ERR_PASSWDMISSMATCH = 464,
	ERR_YOUREBANNEDCREEP = 465,
	ERR_YOUWILLBEBANNED = 466,
	ERR_KEYSET= 467,
	ERR_CHANNELISFULL = 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BANNEDFROMCHAN = 474,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANMASK = 476,
	ERR_NOCHANMODES = 477,
	ERR_BANNLISTFULL = 478,
	ERR_NOPRIVILEGES = 481,
	ERR_CHANOPRIVISNEED = 482,
	ERR_CANTKILLSERVER = 483,
	ERR_RESTRICTED = 484,
	ERR_UNIQOPPRIVSNEEDED = 485,
	ERR_NOOPERHOST = 491,
	ERR_UMODEUNKNOWNFLAG = 501,
	ERR_USERSDONTMATCH = 502

};

struct numeric
{
	IrcNumeric			num;
	const std::string	content;
	numeric(IrcNumeric num, const std::string &content);
};

namespace IrcError
{
	numeric nosuchnick(const std::string &nickname);
	numeric nosuchserver(const std::string &servername);
	numeric nosuchchannel(const std::string &channelname);
	numeric cannotsendtochan(const std::string &channelname);
	numeric toomanychannels(const std::string &channelname);
	numeric wasnosuchnick(const std::string &nickname);
	numeric toomanytargets(const std::string &target);
	numeric noorigin();
	numeric norecipient(const std::string &command);
	numeric notexttosend();
	numeric notoplevel(const std::string &mask);
	numeric wildtoplevel(const std::string &mask);
	numeric unknowncommand(const std::string &command);
	numeric nomotd();
	numeric noadmininfo();
	numeric fillerror(const std::string &file_op, const std::string &file);
	numeric nonicknamegiven();
	numeric erroneusnickname(const std::string &nickname);
	numeric nicknameinuse(const std::string &nickname);
	numeric nickcollision(const std::string &nickname);
	numeric usernotinchannel(const std::string &nickname, const std::string &channel);
	numeric notonchannel(const std::string &channel);
	numeric useronchannel(const std::string &nickname, const std::string &channel);
	numeric nologin(const std::string &user);
	numeric summondisabled();
	numeric usersdisabled();
	numeric notregistered();
	numeric needmoreparams(const std::string &command);
	numeric alreadyregistred();
	numeric nopermforhost();
	numeric passwdmissmatch();
	numeric yourebannedcreep();
	numeric keyset(const std::string &channel);
	numeric channelisfull(const std::string &channel);
	numeric unknownmode(const std::string &c);
	numeric inviteonlychan(const std::string &channel);
	numeric bannedfromchan(const std::string &channel);
	numeric badchannelkey(const std::string &channel);
	numeric noprivileges();
	numeric chanoprisneeded(const std::string &channel);
	numeric cantkillserver();
	numeric nooperhost();
	numeric umodeunknownflag();
	numeric usersdontmatch();
}

namespace IrcReply
{
	std::string welcome(const std::string &userPrefix);
	std::string yourhost(const std::string &servername, const std::string &version);
	std::string created(const std::string &date);
	std::string myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes);
	std::string motdstart(const std::string &servername);
	std::string motd(const std::string &text);
	std::string endofmotd();
}
