#include <iomanip>
#include "Numeric.hpp"
#include "libft.hpp"

namespace IRC
{

Numeric::Numeric(const NumericCode code, const std::string &content) :
code(code), content(content)
{}

namespace Error
{
	const Numeric nosuchnick(const std::string &nickname)
	{
		return (Numeric(ERR_NOSUCHNICK, nickname + " :No such nick/channel"));
	}

	const Numeric nosuchserver(const std::string &servername)
	{
		return (Numeric(ERR_NOSUCHSERVER, servername + " :No such server"));
	}

	const Numeric nosuchchannel(const std::string &channelname)
	{
		return (Numeric(ERR_NOSUCHCHANNEL, channelname + " :No such channel"));
	}

	const Numeric cannotsendtochan(const std::string &channelname)
	{
		return (Numeric(ERR_CANNOTSENDTOCHAN, channelname + " :Cannot send to channel"));
	}

	const Numeric toomanychannels(const std::string &channelname)
	{
		return (Numeric(ERR_TOOMANYCHANNELS, channelname + " :You have joined too many channels"));
	}

	const Numeric wasnosuchnick(const std::string &nickname)
	{
		return (Numeric(ERR_WASNOSUCHNICK, nickname + " :There was no such nickname"));
	}

	const Numeric toomanytargets(const std::string &target)
	{
		return (Numeric(ERR_TOOMANYTARGETS, target + " :Duplicate recipients. No message delivered"));
	}

	const Numeric nosuchservice(const std::string &servicename)
	{
		return (Numeric(ERR_NOSUCHSERVICE, servicename + " :No such service"));
	}

	const Numeric noorigin()
	{
		return (Numeric(ERR_NOORIGIN, ":No origin specified"));
	}

	const Numeric norecipient(const std::string &command)
	{
		return (Numeric(ERR_NORECIPIENT, ":No recipient given " + command));
	}

	const Numeric notexttosend()
	{
		return (Numeric(ERR_NOTEXTTOSEND, ":No text to send"));
	}

	const Numeric notoplevel(const std::string &mask)
	{
		return (Numeric(ERR_NOTOPLEVEL, mask + " :No top level domain specified"));
	}

	const Numeric wildtoplevel(const std::string &mask)
	{
		return (Numeric(ERR_WILDTOPLEVEL, mask + " :Wildcard in top level domain"));
	}

	const Numeric badmask(const std::string &mask)
	{
		return (Numeric(ERR_BADMASK, mask + " :Bad Server/host mask"));
	}

	const Numeric unknowncommand(const std::string &command)
	{
		return (Numeric(ERR_UNKNOWNCOMMAND, command + " :Unknown command"));
	}

	const Numeric nomotd()
	{
		return (Numeric(ERR_NOMOTD, ":MOTD file is missing"));
	}

	const Numeric noadmininfo()
	{
		return (Numeric(ERR_NOADMINFO, ":No administrative info available"));
	}

	const Numeric fillerror(const std::string &file_op, const std::string &file)
	{
		return (Numeric(ERR_FILERROR, ":File error doing " + file_op + " : on " + file));
	}

	const Numeric nonicknamegiven()
	{
		return (Numeric(ERR_NONICKNAMEGIVEN, ":No nickame given"));
	}

	const Numeric erroneusnickname(const std::string &nickname)
	{
		return (Numeric(ERR_ERRONEUSNICKNAME, nickname + " :Erroneus nickname"));
	}

	const Numeric nicknameinuse(const std::string &nickname)
	{
		return (Numeric(ERR_NICKNAMEINUSE, nickname + " :Nickname is already in use"));
	}

	const Numeric nickcollision(const std::string &nickname)
	{
		return (Numeric(ERR_NICKCOLLISION, nickname + " :Nickname collision KILL"));
	}

	const Numeric usernotinchannel(const std::string &nickname, const std::string &channel)
	{
		return (Numeric(ERR_USERNOTINCHANNEL, nickname + ' ' + channel + " :They aren't on that channel"));
	}

	const Numeric notonchannel(const std::string &channel)
	{
		return (Numeric(ERR_NOTONCHANNEL, channel + " :You're not on that channel"));
	}

	const Numeric useronchannel(const std::string &nickname, const std::string &channel)
	{
		return (Numeric(ERR_USERONCHANNEL, nickname + ' ' + channel + " :is already on channel"));
	}

	const Numeric nologin(const std::string &user)
	{
		return (Numeric(ERR_NOLOGIN, user + " :User not logged in"));
	}

	const Numeric summondisabled()
	{
		return (Numeric(ERR_SUMMONDISABLED, ":Summon has been disabled"));
	}

	const Numeric usersdisabled()
	{
		return (Numeric(ERR_USERDISABLED, ":USERS has been disabled"));
	}

	const Numeric notregistered()
	{
		return (Numeric(ERR_NOTREGISTERED, ":You have not registered"));
	}

	const Numeric needmoreparams(const std::string &command)
	{
		return (Numeric(ERR_NEEDMOREPARAMS, command + " :Not enough parameters"));
	}

	const Numeric alreadyregistred()
	{
		return (Numeric(ERR_ALREADYREGISTRED, ":Unauthorized command (already registered)"));
	}

	const Numeric nopermforhost()
	{
		return (Numeric(ERR_NOPERMFORHOST, ":Your host is not among the privileged"));
	}

	const Numeric passwdmissmatch()
	{
		return (Numeric(ERR_PASSWDMISSMATCH, ":Password incorrect"));
	}

	const Numeric yourebannedcreep()
	{
		return (Numeric(ERR_YOUREBANNEDCREEP, ":You are banned from this server"));
	}

	const Numeric keyset(const std::string &channel)
	{
		return (Numeric(ERR_KEYSET, channel + " :Channel key already set"));
	}

	const Numeric channelisfull(const std::string &channel)
	{
		return (Numeric(ERR_CHANNELISFULL, channel + " :Channel is full"));
	}

	const Numeric unknownmode(const std::string &c)
	{
		return (Numeric(ERR_UNKNOWNMODE, c + " :is unknown mode char to me"));
	}

	const Numeric inviteonlychan(const std::string &channel)
	{
		return (Numeric(ERR_INVITEONLYCHAN, channel + " :Cannot join channel (+i)"));
	}

	const Numeric bannedfromchan(const std::string &channel)
	{
		return (Numeric(ERR_BANNEDFROMCHAN, channel + " :Cannot join channel (+b)"));
	}

	const Numeric badchannelkey(const std::string &channel)
	{
		return (Numeric(ERR_BADCHANNELKEY, channel + " :Cannot join channel (+k)"));
	}

	const Numeric nochanmodes(const std::string &channel)
	{
		return (Numeric(ERR_NOCHANMODES, channel + " :Channel doesn't support modes"));
	}

	const Numeric noprivileges()
	{
		return (Numeric(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator"));
	}

	const Numeric chanoprisneeded(const std::string &channel)
	{
		return (Numeric(ERR_CHANOPRIVISNEED, channel + " :You're not channel operator"));
	}

	const Numeric cantkillserver()
	{
		return (Numeric(ERR_CANTKILLSERVER, ":You can't kill a server"));
	}

	const Numeric restricted()
	{
		return (Numeric(ERR_RESTRICTED, ":Your connection is restricted!"));
	}

	const Numeric nooperhost()
	{
		return (Numeric(ERR_NOOPERHOST, ":No O-lines for your host"));
	}

	const Numeric umodeunknownflag()
	{
		return (Numeric(ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag"));
	}

	const Numeric usersdontmatch()
	{
		return (Numeric(ERR_USERSDONTMATCH, ":Can't change mode for other users"));
	}
} /* end of namespace Error */

namespace Reply
{
	const Numeric welcome(const std::string &userPrefix)
	{
		return (Numeric(RPL_WELCOME, ":Welcome to the Internet Relay Network " + userPrefix));
	}

	const Numeric yourhost(const std::string &servername, const std::string &version)
	{
		return (Numeric(RPL_YOURHOST, ":Your host is " + servername + ", running version " + version));
	}

	const Numeric created(const std::string &date)
	{
		return (Numeric(RPL_CREATED, ":This server was created " + date));
	}

	const Numeric myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes)
	{
		return (Numeric(RPL_MYINFO, ':' + servername + ' ' + version + ' ' + availableUserModes + ' ' + availableChannelModes));
	}

	const Numeric bounce(const std::string &servername, const std::string &port)
	{
		return (Numeric(RPL_BOUNCE, ":Try server " + servername + ", port " + port));
	}

	const Numeric statslinkinfo(const std::string &linkname, size_t sendq, size_t sentMessages, size_t sentKbytes, size_t recvMessages, size_t recvKbytes, time_t timeOpen)
	{
		std::stringstream ss;
		ss << linkname << ' ' << sendq << ' ' << sentMessages << ' ' << sentKbytes
		<< ' ' << recvMessages << ' ' << recvKbytes << ' ' << timeOpen;
		return (Numeric(RPL_STATSLINKINFO, ss.str()));
	}

	const Numeric statscommands(const std::string &command, size_t count, size_t byteCount, size_t remoteCount)
	{
		std::stringstream ss;
		ss << command << ' ' << count << ' ' << byteCount << ' ' << remoteCount;
		return (Numeric(RPL_STATSCOMMANDS, ss.str()));
	}

	const Numeric endofstats(const std::string &letter)
	{
		return (Numeric(RPL_ENDOFSTATS, letter + " :End of STATS report"));
	}

	const Numeric umodeis(const std::string &modes)
	{
		return (Numeric(RPL_UMODEIS, '+' + modes));
	}

	const Numeric servlist(const std::string &name, const std::string &server, const std::string &mask, const std::string &type, size_t hopcount, const std::string &info)
	{
		return (Numeric(RPL_SERVLIST, name + ' ' + server + ' ' + mask + ' ' + type + ' ' + ft::toString(hopcount) + " :" + info));
	}

	const Numeric servlistend(const std::string &mask, const std::string &type)
	{
		return (Numeric(RPL_SERVLISTEND, mask + ' ' + type + " :End of service listing"));
	}

	const Numeric statuptime(time_t uptime)
	{
		std::stringstream ss;
		time_t m = 60, h = 60 * m, d = 24 * h;
		ss << ":Server Up " << (uptime / d) << " days ";
		uptime %= d;
		ss << (uptime / h);
		uptime %= h;
		ss << ':' << std::setfill('0') << std::setw(2) << (uptime / m)
		<< ':' << std::setfill('0') << std::setw(2) << (uptime % m);
		return (Numeric(RPL_STATSUPTIME, ss.str()));
	}

	const Numeric statoline(const std::string &hostmask, const std::string &name)
	{
		return (Numeric(RPL_AWAY, "O " + hostmask + " * " + name));
	}

	const Numeric away(const std::string &nickname, const std::string &reason)
	{
		return (Numeric(RPL_AWAY, nickname + " :" + reason));
	}

	const Numeric unaway()
	{
		return (Numeric(RPL_UNAWAY, ":You are no longer marked as being away"));
	}

	const Numeric nowaway()
	{
		return (Numeric(RPL_AWAY, ":You have been marked as being away"));
	}

	const Numeric list(const std::string &channel, size_t visible, const std::string &topic)
	{
		return (Numeric(RPL_LIST, channel + ' ' + ft::toString(visible) + " :" + topic));
	}

	const Numeric listend()
	{
		return (Numeric(RPL_LISTEND, ":End of LIST"));
	}

	const Numeric channelmodeis(const std::string &channel, const std::string &modes)
	{
		return (Numeric(RPL_CHANNELMODEIS, channel + " +" + modes));
	}

	const Numeric notopic(const std::string &channel)
	{
		return (Numeric(RPL_NOTOPIC, channel + " :No topic is set"));
	}

	const Numeric topic(const std::string &channel, const std::string &topic)
	{
		return (Numeric(RPL_TOPIC, channel + " :" + topic));
	}

	const Numeric inviting(const std::string &channel, const std::string &nickname)
	{
		return (Numeric(RPL_INVITING, channel + ' ' + nickname));
	}

	const Numeric invitelist(const std::string &channel, const std::string &invitemask)
	{
		return (Numeric(RPL_INVITELIST, channel + ' ' + invitemask));
	}

	const Numeric endofinvitelist(const std::string &channel)
	{
		return (Numeric(RPL_ENDOFINVITELIST, channel + " :End of channel invite list"));
	}

	const Numeric exceptlist(const std::string &channel, const std::string &exceptionmask)
	{
		return (Numeric(RPL_EXCEPTLIST, channel + ' ' + exceptionmask));
	}

	const Numeric endofexceptlist(const std::string &channel)
	{
		return (Numeric(RPL_ENDOFEXCEPTLIST, channel + " :End of channel exception list"));
	}

	const Numeric version(const std::string &version, const std::string debuglevel, const std::string &server, const std::string &comments)
	{
		return (Numeric(RPL_VERSION, version + '.' + debuglevel + ' ' + server + " :" + comments));
	}

	const Numeric namreply(const std::string &names)
	{
		return (Numeric(RPL_NAMREPLY, names));
	}

	const Numeric endofnames(const std::string &name)
	{
		return (Numeric(RPL_ENDOFNAMES, name + " :End of NAMES list"));
	}

	const Numeric banlist(const std::string &channel, const std::string &banmask)
	{
		return (Numeric(RPL_BANLIST, channel + ' ' + banmask));
	}

	const Numeric endofbanlist(const std::string &channel)
	{
		return (Numeric(RPL_ENDOFBANLIST, channel + " :End of channel ban list"));
	}

	const Numeric motd(const std::string &text)
	{
		return (Numeric(RPL_MOTD, ":- " + text));
	}

	const Numeric motdstart(const std::string &servername)
	{
		return (Numeric(RPL_MOTDSTART, ":- " + servername + " Message of the day - "));
	}

	const Numeric endofmotd()
	{
		return (Numeric(RPL_ENDOFMOTD, ":End of MOTD command"));
	}

	const Numeric info(const std::string &info)
	{
		return (Numeric(RPL_INFO, ':' + info));
	}

	const Numeric endofinfo()
	{
		return (Numeric(RPL_ENDOFINFO, ":End of INFO list"));
	}

	const Numeric youreoper()
	{
		return (Numeric(RPL_YOUREOPER, "You are now an IRC operator"));
	}

	const Numeric youreservice(const std::string &servicename)
	{
		return (Numeric(RPL_YOURESERVICE, "You are service " + servicename));
	}

	const Numeric time()
	{
		return (Numeric(RPL_TIME, ft::toDate(::time(NULL), "%x - %I:%M:%S %z")));
	}

	const Numeric lusersclient(size_t users, size_t services, size_t servers)
	{
		return (Numeric(RPL_LUSERCLIENT, ":There are " + ft::toString(users) + " users and " + ft::toString(services) + " services on " + ft::toString(servers) + " server"));	
	}

	const Numeric lusersop(size_t nb)
	{
		return (Numeric(RPL_LUSEROP, ft::toString(nb) + " :operator(s) online"));
	}

	const Numeric lusersunknown(size_t nb)
	{
		return (Numeric(RPL_LUSERUNKNOWN, ft::toString(nb) + " :unknown connection(s)"));
	}

	const Numeric luserschannels(size_t nb)
	{
		return (Numeric(RPL_LUSERCHANNELS, ft::toString(nb) + " :channels formed"));
	}

	const Numeric lusersme(size_t nb)
	{
		return (Numeric(RPL_LUSERME, ":I have " + ft::toString(nb) + " clients and 1 server"));
	}

	const Numeric whoisuser(const std::string &nick, const std::string &user, const std::string &host, const std::string &realname)
	{
		return (Numeric(RPL_WHOISUSER, nick + " " + user + " " + host + " * :" + realname));
	}

	const Numeric whoisserver(const std::string &nick, const std::string &servername, const std::string shortinfo)
	{
		return (Numeric(RPL_WHOISSERVER, nick + " " + servername + " :" + shortinfo));
	}

	const Numeric whoisoperator(const std::string &nick)
	{
		return (Numeric(RPL_WHOISOPERATOR, nick + " :is an IRC operator"));
	}

	const Numeric whoisidle(const std::string &nick, size_t idle)
	{
		return (Numeric(RPL_WHOISIDLE, nick + " " + ft::toString(idle) + " :seconds idle"));
	}

	const Numeric whoischannels(const std::string &nick, const std::string &status, const std::string &channel)
	{
		return (Numeric(RPL_WHOISCHANNELS, nick + " :" + status + channel));
	}

	const Numeric endofwhois(const std::string &nick)
	{
		return (Numeric(RPL_ENDOFWHOIS, nick + " :End of WHOIS list"));
	}

	const Numeric whoreply(const std::string &msg)
	{
		return (Numeric(RPL_WHOREPLY, msg));
	}

	const Numeric endofwho(const std::string &target)
	{
		return (Numeric(RPL_ENDOFWHO, target + " :End of WHO list"));
	}

	const Numeric whowasuser(const std::string &nick, const std::string &user, const std::string &host, const std::string &realname)
	{
		return (Numeric(RPL_WHOWASUSER, nick + " " + user + " " + host + " * :" + realname));
	}

	const Numeric endofwhowas(const std::string &nick)
	{
		return (Numeric(RPL_ENDOFWHOWAS, nick + " :End of WHOWAS"));
	}

	const Numeric rehashing(const std::string &filename)
	{
		return (Numeric(RPL_REHASHING, filename + ": Rehashing"));
	}

	const Numeric userhostreply(const std::string &msg)
	{
		return (Numeric(RPL_USERHOST, msg));
	}
} /* end of namespace Reply */

} /* end of namespace IRC */
