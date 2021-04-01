/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumeric.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:37:07 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/01 18:50:30 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"

IrcNumeric::IrcNumeric(IrcNumericCode code, const std::string &content):code(code), content(content) {}

namespace IrcError
{
	IrcNumeric nosuchnick(const std::string &nickname) {
		return (IrcNumeric(ERR_NOSUCHNICK, nickname + " :No such nick/channel"));
	}
	IrcNumeric nosuchserver(const std::string &servername) {
		return (IrcNumeric(ERR_NOSUCHSERVER, servername + " :No such server"));
	}
	IrcNumeric nosuchchannel(const std::string &channelname) {
		return (IrcNumeric(ERR_NOSUCHCHANNEL, channelname + " :No such channel"));
	}
	IrcNumeric cannotsendtochan(const std::string &channelname) {
		return (IrcNumeric(ERR_CANNOTSENDTOCHAN, channelname + " :Cannot send to channel"));
	}
	IrcNumeric toomanychannels(const std::string &channelname) {
		return (IrcNumeric(ERR_TOOMANYCHANNELS, channelname + " :You have joined too many channels"));
	}
	IrcNumeric wasnosuchnick(const std::string &nickname) {
		return (IrcNumeric(ERR_WASNOSUCHNICK, nickname + " :There was no such nickname"));
	}
	IrcNumeric toomanytargets(const std::string &target) {
		return (IrcNumeric(ERR_TOOMANYTARGETS, target + " :Duplicate recipients. No message delivered"));
	}
	IrcNumeric noorigin() {
		return (IrcNumeric(ERR_NOORIGIN, ":No origin specified"));
	}
	IrcNumeric norecipient(const std::string &command) {
		return (IrcNumeric(ERR_NORECIPIENT, ":No recipient given " + command));
	}
	IrcNumeric notexttosend() {
		return (IrcNumeric(ERR_NOTEXTTOSEND, ":No text to send"));
	}
	IrcNumeric notoplevel(const std::string &mask) {
		return (IrcNumeric(ERR_NOTOPLEVEL, mask + " :No top level domain specified"));
	}
	IrcNumeric wildtoplevel(const std::string &mask) {
		return (IrcNumeric(ERR_WILDTOPLEVEL, mask + " :Wildcard in top level domain"));
	}
	IrcNumeric unknowncommand(const std::string &command) {
		return (IrcNumeric(ERR_UNKNOWNCOMMAND, command + " :Unknown command"));
	}
	IrcNumeric nomotd() {
		return (IrcNumeric(ERR_NOMOTD, ":MOTD file is missing"));
	}
	IrcNumeric noadmininfo() {
		return (IrcNumeric(ERR_NOADMINFO, ":No administrative info available"));
	}
	IrcNumeric fillerror(const std::string &file_op, const std::string &file) {
		return (IrcNumeric(ERR_FILERROR, ":File error doing " + file_op + " : on " + file));
	}
	IrcNumeric nonicknamegiven() {
		return (IrcNumeric(ERR_NONICKNAMEGIVEN, ":No nickame given"));
	}
	IrcNumeric erroneusnickname(const std::string &nickname) {
		return (IrcNumeric(ERR_ERRONEUSNICKNAME, nickname + " :Erroneus nickname"));
	}
	IrcNumeric nicknameinuse(const std::string &nickname) {
		return (IrcNumeric(ERR_NICKNAMEINUSE, nickname + " :Nickname is already in use"));
	}
	IrcNumeric nickcollision(const std::string &nickname) {
		return (IrcNumeric(ERR_NICKCOLLISION, nickname + " :Nickname collision KILL"));
	}
	IrcNumeric usernotinchannel(const std::string &nickname, const std::string &channel) {
		return (IrcNumeric(ERR_USERNOTINCHANNEL, nickname + ' ' + channel + " :They aren't on that channel"));
	}
	IrcNumeric notonchannel(const std::string &channel) {
		return (IrcNumeric(ERR_NOTONCHANNEL, channel + " :You're not on that channel"));
	}
	IrcNumeric useronchannel(const std::string &nickname, const std::string &channel) {
		return (IrcNumeric(ERR_USERONCHANNEL, nickname + ' ' + channel + " :is already on channel"));
	}
	IrcNumeric nologin(const std::string &user) {
		return (IrcNumeric(ERR_NOLOGIN, user + " :User not logged in"));
	}
	IrcNumeric summondisabled() {
		return (IrcNumeric(ERR_SUMMONDISABLED, ":Summon has been disabled"));
	}
	IrcNumeric usersdisabled() {
		return (IrcNumeric(ERR_USERDISABLED, ":USERS has been disabled"));
	}
	IrcNumeric notregistered() {
		return (IrcNumeric(ERR_NOTREGISTERED, ":You have not registered"));
	}
	IrcNumeric needmoreparams(const std::string &command) {
		return (IrcNumeric(ERR_NEEDMOREPARAMS, command + " :Not enough parameters"));
	}
	IrcNumeric alreadyregistred() {
		return (IrcNumeric(ERR_ALREADYREGISTERED, ":You may not reregister"));
	}
	IrcNumeric nopermforhost() {
		return (IrcNumeric(ERR_NOPERMFORHOST, ":Your host is not among the privileged"));
	}
	IrcNumeric passwdmissmatch() {
		return (IrcNumeric(ERR_PASSWDMISSMATCH, ":Password incorrect"));
	}
	IrcNumeric yourebannedcreep() {
		return (IrcNumeric(ERR_YOUREBANNEDCREEP, ":You are banned from this server"));
	}
	IrcNumeric keyset(const std::string &channel) {
		return (IrcNumeric(ERR_KEYSET, channel + " :Channel key already set"));
	}
	IrcNumeric channelisfull(const std::string &channel) {
		return (IrcNumeric(ERR_CHANNELISFULL, channel + " :Channel is full"));
	}
	IrcNumeric unknownmode(const std::string &c) {
		return (IrcNumeric(ERR_UNKNOWNMODE, c + " :is unknown mode char to me"));
	}
	IrcNumeric inviteonlychan(const std::string &channel) {
		return (IrcNumeric(ERR_INVITEONLYCHAN, channel + " :Cannot join channel (+i)"));
	}
	IrcNumeric bannedfromchan(const std::string &channel) {
		return (IrcNumeric(ERR_BANNEDFROMCHAN, channel + " :Cannot join channel (+b)"));
	}
	IrcNumeric badchannelkey(const std::string &channel) {
		return (IrcNumeric(ERR_BADCHANNELKEY, channel + " :Cannot join channel (+k)"));
	}
	IrcNumeric noprivileges() {
		return (IrcNumeric(ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator"));
	}
	IrcNumeric chanoprisneeded(const std::string &channel) {
		return (IrcNumeric(ERR_CHANOPRIVISNEED, channel + " :You're not channel operator"));
	}
	IrcNumeric cantkillserver() {
		return (IrcNumeric(ERR_CANTKILLSERVER, ":You can't kill a server"));
	}
	IrcNumeric nooperhost() {
		return (IrcNumeric(ERR_NOPERMFORHOST, ":No O-lines for your host"));
	}
	IrcNumeric umodeunknownflag() {
		return (IrcNumeric(ERR_UMODEUNKNOWNFLAG, ":Unknown MODE flag"));
	}
	IrcNumeric usersdontmatch() {
		return (IrcNumeric(ERR_USERSDONTMATCH, ":Can't change mode for other users"));
	}
}


namespace IrcReply
{
	IrcNumeric welcome(const std::string &userPrefix)
	{
		return (IrcNumeric(RPL_WELCOME, ":Welcome to the Internet Relay Network " + userPrefix));
	}

	IrcNumeric yourhost(const std::string &servername, const std::string &version)
	{
		return (IrcNumeric(RPL_YOURHOST, ":Your host is " + servername + ", running version " + version));
	}

	IrcNumeric created(const std::string &date)
	{
		return (IrcNumeric(RPL_CREATED, ":This server was created " + date));
	}

	IrcNumeric myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes)
	{
		return (IrcNumeric(RPL_MYINFO, ':' + servername + ' ' + version + ' ' + availableUserModes + ' ' + availableChannelModes));
	}

	IrcNumeric bounce(const std::string &servername, const std::string &port)
	{
		return (IrcNumeric(RPL_BOUNCE, ":Try server " + servername + ", port " + port));
	}

	IrcNumeric motd(const std::string &text)
	{
		return (IrcNumeric(RPL_MOTD, ":- " + text));
	}

	IrcNumeric motdstart(const std::string &servername)
	{
		return (IrcNumeric(RPL_MOTDSTART, ":- " + servername + " Message of the day - "));
	}

	IrcNumeric endofmotd()
	{
		return (IrcNumeric(RPL_ENDOFMOTD, ":End of MOTD command"));
	}
}
