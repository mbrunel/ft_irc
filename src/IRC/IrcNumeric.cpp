#include "IrcNumeric.hpp"
#include "ft.hpp"

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

	const IrcNumeric badmask(const std::string &mask)
	{
		return (IrcNumeric(ERR_BADMASK, mask + " :Bad Server/host mask"));
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

	const IrcNumeric restricted()
	{
		return (IrcNumeric(ERR_RESTRICTED, ":Your connection is restricted!"));
	}

	const IrcNumeric nooperhost()
	{
		return (IrcNumeric(ERR_NOOPERHOST, ":No O-lines for your host"));
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

	const IrcNumeric list(const std::string &channel, size_t visible, const std::string &topic)
	{
		return (IrcNumeric(RPL_LIST, channel + ' ' + ft::to_string(visible) + " :" + topic));
	}

	const IrcNumeric listend()
	{
		return (IrcNumeric(RPL_LISTEND, ":End of LIST"));
	}

	const IrcNumeric channelmodeis(const std::string &channel, const std::string &modes)
	{
		return (IrcNumeric(RPL_CHANNELMODEIS, channel + " +" + modes));
	}

	const IrcNumeric notopic(const std::string &channel)
	{
		return (IrcNumeric(RPL_NOTOPIC, channel + " :No topic is set"));
	}

	const IrcNumeric topic(const std::string &channel, const std::string &topic)
	{
		return (IrcNumeric(RPL_TOPIC, channel + " :" + topic));
	}

	const IrcNumeric inviting(const std::string &channel, const std::string &nickname)
	{
		return (IrcNumeric(RPL_INVITING, channel + ' ' + nickname));
	}

	const IrcNumeric invitelist(const std::string &channel, const std::string &invitemask)
	{
		return (IrcNumeric(RPL_INVITELIST, channel + ' ' + invitemask));
	}

	const IrcNumeric endofinvitelist(const std::string &channel)
	{
		return (IrcNumeric(RPL_ENDOFINVITELIST, channel + " :End of channel invite list"));
	}

	const IrcNumeric exceptlist(const std::string &channel, const std::string &exceptionmask)
	{
		return (IrcNumeric(RPL_EXCEPTLIST, channel + ' ' + exceptionmask));
	}

	const IrcNumeric endofexceptlist(const std::string &channel)
	{
		return (IrcNumeric(RPL_ENDOFEXCEPTLIST, channel + " :End of channel exception list"));
	}

	const IrcNumeric version(const std::string &version, const std::string debuglevel, const std::string &server, const std::string &comments)
	{
		return (IrcNumeric(RPL_VERSION, version + '.' + debuglevel + ' ' + server + " :" + comments));
	}

	const IrcNumeric namreply(const std::string &names)
	{
		return (IrcNumeric(RPL_NAMREPLY, names));
	}

	const IrcNumeric endofnames(const std::string &name)
	{
		return (IrcNumeric(RPL_ENDOFNAMES, name + " :End of NAMES list"));
	}

	const IrcNumeric banlist(const std::string &channel, const std::string &banmask)
	{
		return (IrcNumeric(RPL_BANLIST, channel + ' ' + banmask));
	}

	const IrcNumeric endofbanlist(const std::string &channel)
	{
		return (IrcNumeric(RPL_ENDOFBANLIST, channel + " :End of channel ban list"));
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
		return (IrcNumeric(RPL_TIME, ft::to_date(::time(NULL), "%x - %I:%M:%S")));
	}

	const IrcNumeric lusersclient(int nb)
	{
		return (IrcNumeric(RPL_LUSERCLIENT, ":There are " + ft::to_string(nb) + " users on 1 server"));	
	}

	const IrcNumeric lusersop(int nb)
	{
		return (IrcNumeric(RPL_LUSEROP, ft::to_string(nb) + " :operator(s) online"));
	}

	const IrcNumeric lusersunknown(int nb)
	{
		return (IrcNumeric(RPL_LUSERUNKNOWN, ft::to_string(nb) + " :unknown connection(s)"));
	}

	const IrcNumeric luserschannels(int nb)
	{
		return (IrcNumeric(RPL_LUSERCHANNELS, ft::to_string(nb) + " :channels formed"));
	}

	const IrcNumeric lusersme(int nb)
	{
		return (IrcNumeric(RPL_LUSERME, ":I have " + ft::to_string(nb) + " clients and 1 server"));
	}

	const IrcNumeric whoreply(const std::string &msg)
	{
		return (IrcNumeric(RPL_WHOREPLY, msg));
	}

	const IrcNumeric endofwho(const std::string &target)
	{
		return (IrcNumeric(RPL_ENDOFWHO, target + " :End of WHO list"));
	}
}
