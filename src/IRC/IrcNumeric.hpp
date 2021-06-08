/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumeric.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 15:25:14 by asoursou          #+#    #+#             */
/*   Updated: 2021/06/08 19:55:07 by mapapin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>

enum IrcNumericCode
{
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
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_NOPERMFORHOST = 463,
	ERR_PASSWDMISSMATCH = 464,
	ERR_YOUREBANNEDCREEP = 465,
	ERR_YOUWILLBEBANNED = 466,
	ERR_KEYSET = 467,
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
	ERR_USERSDONTMATCH = 502,

	RPL_WELCOME = 1,
	RPL_YOURHOST = 2,
	RPL_CREATED = 3,
	RPL_MYINFO = 4,
	RPL_BOUNCE = 5,
	RPL_UMODEIS = 221,
	RPL_LUSERCLIENT = 251,
	RPL_LUSEROP = 252,
	RPL_LUSERUNKNOWN = 253,
	RPL_LUSERCHANNELS = 254,
	RPL_LUSERME = 255,
	RPL_AWAY = 301,
	RPL_UNAWAY = 305,
	RPL_NOWAWAY = 306,
	RPL_LISTSTART = 321, /* Not used / (╯°□°）╯︵ ┻━┻  */
	RPL_LIST = 322,
	RPL_LISTEND = 323,
	RPL_CHANNELMODEIS = 324,
	RPL_NOTOPIC = 331,
	RPL_TOPIC = 332,
	RPL_INVITING = 341,
	RPL_INVITELIST = 346,
	RPL_ENDOFINVITELIST =  347,
	RPL_EXCEPTLIST = 348,
	RPL_ENDOFEXCEPTLIST = 349,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_BANLIST = 367,
	RPL_ENDOFBANLIST = 368,
	RPL_MOTD = 372,
	RPL_MOTDSTART = 375,
	RPL_ENDOFMOTD = 376,
	RPL_YOUREOPER = 381,
	RPL_TIME = 391
};

struct IrcNumeric
{
	const IrcNumericCode	code;
	const std::string		content;
	IrcNumeric(const IrcNumericCode code, const std::string &content);
};

namespace IrcError
{
	const IrcNumeric nosuchnick(const std::string &nickname);
	const IrcNumeric nosuchserver(const std::string &servername);
	const IrcNumeric nosuchchannel(const std::string &channelname);
	const IrcNumeric cannotsendtochan(const std::string &channelname);
	const IrcNumeric toomanychannels(const std::string &channelname);
	const IrcNumeric wasnosuchnick(const std::string &nickname);
	const IrcNumeric toomanytargets(const std::string &target);
	const IrcNumeric noorigin();
	const IrcNumeric norecipient(const std::string &command);
	const IrcNumeric notexttosend();
	const IrcNumeric notoplevel(const std::string &mask);
	const IrcNumeric wildtoplevel(const std::string &mask);
	const IrcNumeric unknowncommand(const std::string &command);
	const IrcNumeric nomotd();
	const IrcNumeric noadmininfo();
	const IrcNumeric fillerror(const std::string &file_op, const std::string &file);
	const IrcNumeric nonicknamegiven();
	const IrcNumeric erroneusnickname(const std::string &nickname);
	const IrcNumeric nicknameinuse(const std::string &nickname);
	const IrcNumeric nickcollision(const std::string &nickname);
	const IrcNumeric usernotinchannel(const std::string &nickname, const std::string &channel);
	const IrcNumeric notonchannel(const std::string &channel);
	const IrcNumeric useronchannel(const std::string &nickname, const std::string &channel);
	const IrcNumeric nologin(const std::string &user);
	const IrcNumeric summondisabled();
	const IrcNumeric usersdisabled();
	const IrcNumeric notregistered();
	const IrcNumeric needmoreparams(const std::string &command);
	const IrcNumeric alreadyregistred();
	const IrcNumeric nopermforhost();
	const IrcNumeric passwdmissmatch();
	const IrcNumeric yourebannedcreep();
	const IrcNumeric keyset(const std::string &channel);
	const IrcNumeric channelisfull(const std::string &channel);
	const IrcNumeric unknownmode(const std::string &c);
	const IrcNumeric inviteonlychan(const std::string &channel);
	const IrcNumeric bannedfromchan(const std::string &channel);
	const IrcNumeric badchannelkey(const std::string &channel);
	const IrcNumeric nochanmodes(const std::string &channel);
	const IrcNumeric noprivileges();
	const IrcNumeric chanoprisneeded(const std::string &channel);
	const IrcNumeric cantkillserver();
	const IrcNumeric nooperhost();
	const IrcNumeric umodeunknownflag();
	const IrcNumeric usersdontmatch();
}

namespace IrcReply
{
	const IrcNumeric welcome(const std::string &userPrefix);
	const IrcNumeric yourhost(const std::string &servername, const std::string &version);
	const IrcNumeric created(const std::string &date);
	const IrcNumeric myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes);
	const IrcNumeric bounce(const std::string &servername, const std::string &port);
	const IrcNumeric umodeis(const std::string &modes);
	const IrcNumeric away(const std::string &nickname, const std::string &reason);
	const IrcNumeric unaway();
	const IrcNumeric nowaway();
	const IrcNumeric list(const std::string &channel, size_t visible, const std::string &topic);
	const IrcNumeric listend();
	const IrcNumeric channelmodeis(const std::string &channel, const std::string &modes);
	const IrcNumeric notopic(const std::string &channel);
	const IrcNumeric topic(const std::string &channel, const std::string &topic);
	const IrcNumeric inviting(const std::string &channel, const std::string &nickname);
	const IrcNumeric invitelist(const std::string &channel, const std::string &invitemask);
	const IrcNumeric endofinvitelist(const std::string &channel);
	const IrcNumeric exceptlist(const std::string &channel, const std::string &exceptionmask);
	const IrcNumeric endofexceptlist(const std::string &channel);
	const IrcNumeric namreply(const std::string &names);
	const IrcNumeric endofnames(const std::string &name);
	const IrcNumeric banlist(const std::string &channel, const std::string &banmask);
	const IrcNumeric endofbanlist(const std::string &channel);
	const IrcNumeric motd(const std::string &text);
	const IrcNumeric motdstart(const std::string &servername);
	const IrcNumeric endofmotd();
	const IrcNumeric youreoper();
	const IrcNumeric time();
	const IrcNumeric lusersclient(int nb);
	const IrcNumeric lusersop(int nb);
	const IrcNumeric lusersunknown(int nb);
	const IrcNumeric luserschannels(int nb);
	const IrcNumeric lusersme(int nb);
}
