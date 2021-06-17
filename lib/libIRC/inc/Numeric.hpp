#pragma once
#include <string>

namespace IRC
{

enum NumericCode
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
	RPL_STATSLINKINFO = 211,
	RPL_STATSCOMMANDS = 212,
	RPL_ENDOFSTATS = 219,
	RPL_UMODEIS = 221,
	RPL_SERVLIST = 234,
	RPL_SERVLISTEND = 235,
	RPL_STATSUPTIME = 242,
	RPL_STATSOLINE = 243,
	RPL_LUSERCLIENT = 251,
	RPL_LUSEROP = 252,
	RPL_LUSERUNKNOWN = 253,
	RPL_LUSERCHANNELS = 254,
	RPL_LUSERME = 255,
	RPL_AWAY = 301,
	RPL_UNAWAY = 305,
	RPL_NOWAWAY = 306,
	RPL_WHOISUSER = 311,
	RPL_WHOISSERVER = 312,
	RPL_WHOISOPERATOR = 313,
	RPL_WHOISIDLE = 317,
	RPL_ENDOFWHOIS = 318,
	RPL_WHOISCHANNELS = 319,
	RPL_ENDOFWHO = 315,
	RPL_WHOWASUSER = 314,
	RPL_ENDOFWHOWAS = 369,
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
	RPL_VERSION = 351,
	RPL_WHOREPLY = 352,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_BANLIST = 367,
	RPL_ENDOFBANLIST = 368,
	RPL_INFO = 371,
	RPL_MOTD = 372,
	RPL_ENDOFINFO = 374,
	RPL_MOTDSTART = 375,
	RPL_ENDOFMOTD = 376,
	RPL_YOUREOPER = 381,
	RPL_YOURESERVICE = 383,
	RPL_TIME = 391
};

struct Numeric
{
	const NumericCode	code;
	const std::string	content;
	Numeric(const NumericCode code, const std::string &content);
};

namespace Error
{
	const Numeric nosuchnick(const std::string &nickname);
	const Numeric nosuchserver(const std::string &servername);
	const Numeric nosuchchannel(const std::string &channelname);
	const Numeric cannotsendtochan(const std::string &channelname);
	const Numeric toomanychannels(const std::string &channelname);
	const Numeric wasnosuchnick(const std::string &nickname);
	const Numeric toomanytargets(const std::string &target);
	const Numeric noorigin();
	const Numeric norecipient(const std::string &command);
	const Numeric notexttosend();
	const Numeric notoplevel(const std::string &mask);
	const Numeric badmask(const std::string &mask);
	const Numeric wildtoplevel(const std::string &mask);
	const Numeric unknowncommand(const std::string &command);
	const Numeric nomotd();
	const Numeric noadmininfo();
	const Numeric fillerror(const std::string &file_op, const std::string &file);
	const Numeric nonicknamegiven();
	const Numeric erroneusnickname(const std::string &nickname);
	const Numeric nicknameinuse(const std::string &nickname);
	const Numeric nickcollision(const std::string &nickname);
	const Numeric usernotinchannel(const std::string &nickname, const std::string &channel);
	const Numeric notonchannel(const std::string &channel);
	const Numeric useronchannel(const std::string &nickname, const std::string &channel);
	const Numeric nologin(const std::string &user);
	const Numeric summondisabled();
	const Numeric usersdisabled();
	const Numeric notregistered();
	const Numeric needmoreparams(const std::string &command);
	const Numeric alreadyregistred();
	const Numeric nopermforhost();
	const Numeric passwdmissmatch();
	const Numeric yourebannedcreep();
	const Numeric keyset(const std::string &channel);
	const Numeric channelisfull(const std::string &channel);
	const Numeric unknownmode(const std::string &c);
	const Numeric inviteonlychan(const std::string &channel);
	const Numeric bannedfromchan(const std::string &channel);
	const Numeric badchannelkey(const std::string &channel);
	const Numeric nochanmodes(const std::string &channel);
	const Numeric noprivileges();
	const Numeric chanoprisneeded(const std::string &channel);
	const Numeric cantkillserver();
	const Numeric restricted();
	const Numeric nooperhost();
	const Numeric umodeunknownflag();
	const Numeric usersdontmatch();
} /* end of namespace Error */

namespace Reply
{
	const Numeric welcome(const std::string &userPrefix);
	const Numeric yourhost(const std::string &servername, const std::string &version);
	const Numeric created(const std::string &date);
	const Numeric myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes);
	const Numeric bounce(const std::string &servername, const std::string &port);
	const Numeric statslinkinfo(const std::string &linkname, size_t sendq, size_t sentMessages, size_t sentKbytes, size_t recvMessages, size_t recvKbytes, time_t timeOpen);
	const Numeric statscommands(const std::string &command, size_t count, size_t byteCount, size_t remoteCount);
	const Numeric endofstats(const std::string &letter);
	const Numeric umodeis(const std::string &modes);
	const Numeric servlist(const std::string &name, const std::string &server, const std::string &mask, const std::string &type, size_t hopcount, const std::string &info);
	const Numeric servlistend(const std::string &mask, const std::string &type);
	const Numeric statuptime(time_t uptime);
	const Numeric statoline(const std::string &hostmask, const std::string &name);
	const Numeric away(const std::string &nickname, const std::string &reason);
	const Numeric unaway();
	const Numeric nowaway();
	const Numeric list(const std::string &channel, size_t visible, const std::string &topic);
	const Numeric listend();
	const Numeric channelmodeis(const std::string &channel, const std::string &modes);
	const Numeric notopic(const std::string &channel);
	const Numeric topic(const std::string &channel, const std::string &topic);
	const Numeric inviting(const std::string &channel, const std::string &nickname);
	const Numeric invitelist(const std::string &channel, const std::string &invitemask);
	const Numeric endofinvitelist(const std::string &channel);
	const Numeric exceptlist(const std::string &channel, const std::string &exceptionmask);
	const Numeric endofexceptlist(const std::string &channel);
	const Numeric version(const std::string &version, const std::string debuglevel, const std::string &server, const std::string &comments);
	const Numeric namreply(const std::string &names);
	const Numeric endofnames(const std::string &name);
	const Numeric banlist(const std::string &channel, const std::string &banmask);
	const Numeric endofbanlist(const std::string &channel);
	const Numeric motd(const std::string &text);
	const Numeric motdstart(const std::string &servername);
	const Numeric endofmotd();
	const Numeric info(const std::string &info);
	const Numeric endofinfo();
	const Numeric youreoper();
	const Numeric youreservice(const std::string &servicename);
	const Numeric time();
	const Numeric lusersclient(size_t nb);
	const Numeric lusersop(size_t nb);
	const Numeric lusersunknown(size_t nb);
	const Numeric luserschannels(size_t nb);
	const Numeric lusersme(size_t nb);
	const Numeric whoreply(const std::string &msg);
	const Numeric endofwho(const std::string &target);
	const Numeric whoisuser(const std::string &nick, const std::string &user, const std::string &host, const std::string &realname);
	const Numeric whoisserver(const std::string &nick, const std::string &servername, const std::string shortinfo);
	const Numeric whoisoperator(const std::string &nick);
	const Numeric whoisidle(const std::string &nick, size_t idle);
	const Numeric whoischannels(const std::string &nick, const std::string &status, const std::string &chan);
	const Numeric endofwhois(const std::string &nick);
	const Numeric whoreply(const std::string &msg);
	const Numeric endofwho(const std::string &target);
	const Numeric whowasuser(const std::string &nick, const std::string &user, const std::string &host, const std::string &realname);
	const Numeric endofwhowas(const std::string &nick);
} /* end of namespace Reply */

} /* end of namespace IRC */
