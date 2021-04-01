/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumeric.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:37:07 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/01 17:35:14 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"

numeric::numeric(IrcNumeric num, const std::string &content):num(num), content(content) {}

namespace IrcError
{
	numeric nosuchnick(const std::string &nickname) {
		return (numeric(ERR_NOSUCHNICK, nickname + "No such nick/channel"));
	}
	numeric nosuchserver(const std::string &servername) {
		return (numeric(ERR_NOSUCHSERVER, servername + "No such server"));
	}
	numeric nosuchchannel(const std::string &channelname) {
		return (numeric(ERR_NOSUCHCHANNEL, channelname + "No such channel"));
	}
	numeric cannotsendtochan(const std::string &channelname) {
		return (numeric(ERR_CANNOTSENDTOCHAN, channelname + "Cannot send to channel"));
	}
	numeric toomanychannels(const std::string &channelname) {
		return (numeric(ERR_TOOMANYCHANNELS, channelname + "You have joined too many channels"));
	}
	numeric wasnosuchnick(const std::string &nickname) {
		return (numeric(ERR_WASNOSUCHNICK, nickname + "There was no such nickname"));
	}
	numeric toomanytargets(const std::string &target) {
		return (numeric(ERR_TOOMANYTARGETS, target + "Duplicate recipients. No message delivered"));
	}
	numeric noorigin() {
		return (numeric(ERR_NOORIGIN, "No origin specified"));
	}
	numeric norecipient(const std::string &command) {
		return (numeric(ERR_NORECIPIENT, "No recipient given " + command));
	}
	numeric notexttosend() {
		return (numeric(ERR_NOTEXTTOSEND, "No text to send"));
	}
	numeric notoplevel(const std::string &mask) {
		return (numeric(ERR_NOTOPLEVEL, mask + "No top level domain specified"));
	}
	numeric wildtoplevel(const std::string &mask) {
		return (numeric(ERR_WILDTOPLEVEL, mask + "Wildcard in top level domain"));
	}
	numeric unknowncommand(const std::string &command) {
		return (numeric(ERR_UNKNOWNCOMMAND, command + "Unknown command"));
	}
	numeric nomotd() {
		return (numeric(ERR_NOMOTD, "MOTD file is missing"));
	}
	numeric noadmininfo() {
		return (numeric(ERR_NOADMINFO, "No administrative info available"));
	}
	numeric fillerror(const std::string &file_op, const std::string &file) {
		return (numeric(ERR_FILERROR, "File error doing " + file_op + " on " + file));
	}
	numeric nonicknamegiven() {
		return (numeric(ERR_NONICKNAMEGIVEN, "No nickame given"));
	}
	numeric erroneusnickname(const std::string &nickname) {
		return (numeric(ERR_ERRONEUSNICKNAME, nickname + "Erroneus nickname"));
	}
	numeric nicknameinuse(const std::string &nickname) {
		return (numeric(ERR_NICKNAMEINUSE, nickname + "Nickname is already in use"));
	}
	numeric nickcollision(const std::string &nickname) {
		return (numeric(ERR_NICKCOLLISION, nickname + "Nickname collision KILL"));
	}
	numeric usernotinchannel(const std::string &nickname, const std::string &channel) {
		return (numeric(ERR_USERNOTINCHANNEL, nickname + " " + channel + "They aren't on that channel"));
	}
	numeric notonchannel(const std::string &channel) {
		return (numeric(ERR_NOTONCHANNEL, channel + "You're not on that channel"));
	}
	numeric useronchannel(const std::string &nickname, const std::string &channel) {
		return (numeric(ERR_USERONCHANNEL, nickname + " " + channel + "is already on channel"));
	}
	numeric nologin(const std::string &user) {
		return (numeric(ERR_NOLOGIN, user + "User not logged in"));
	}
	numeric summondisabled() {
		return (numeric(ERR_SUMMONDISABLED, "Summon has been disabled"));
	}
	numeric usersdisabled() {
		return (numeric(ERR_USERDISABLED, "USERS has been disabled"));
	}
	numeric notregistered() {
		return (numeric(ERR_NOTREGISTERED, ":You have not registered"));
	}
	numeric needmoreparams(const std::string &command) {
		return (numeric(ERR_NEEDMOREPARAMS, command + "Not enough parameters"));
	}
	numeric alreadyregistred() {
		return (numeric(ERR_ALREADYREGISTERED, "You may not reregister"));
	}
	numeric nopermforhost() {
		return (numeric(ERR_NOPERMFORHOST, "Your host is not among the privileged"));
	}
	numeric passwdmissmatch() {
		return (numeric(ERR_PASSWDMISSMATCH, "Password incorrect"));
	}
	numeric yourebannedcreep() {
		return (numeric(ERR_YOUREBANNEDCREEP, "you're banned from this server"));
	}
	numeric keyset(const std::string &channel) {
		return (numeric(ERR_KEYSET, channel + "Channel key already set"));
	}
	numeric channelisfull(const std::string &channel) {
		return (numeric(ERR_CHANNELISFULL, channel + "Channel is full"));
	}
	numeric unknownmode(const std::string &c) {
		return (numeric(ERR_UNKNOWNMODE, c + "is unknown mode char to me"));
	}
	numeric inviteonlychan(const std::string &channel) {
		return (numeric(ERR_INVITEONLYCHAN, channel + "Cannot join channel (+i)"));
	}
	numeric bannedfromchan(const std::string &channel) {
		return (numeric(ERR_BANNEDFROMCHAN, channel + "Cannot join channel (+b)"));
	}
	numeric badchannelkey(const std::string &channel) {
		return (numeric(ERR_BADCHANNELKEY, channel + "Cannot join channel (+k)"));
	}
	numeric noprivileges() {
		return (numeric(ERR_NOPRIVILEGES, "Permission Denied- You're not an IRC operator"));
	}
	numeric chanoprisneeded(const std::string &channel) {
		return (numeric(ERR_CHANOPRIVISNEED, channel + "You're not channel operator"));
	}
	numeric cantkillserver() {
		return (numeric(ERR_CANTKILLSERVER, "You can't kill a server"));
	}
	numeric nooperhost() {
		return (numeric(ERR_NOPERMFORHOST, "No O-lines for your host"));
	}
	numeric umodeunknownflag() {
		return (numeric(ERR_UMODEUNKNOWNFLAG, "Unknown MODE flag"));
	}
	numeric usersdontmatch() {
		return (numeric(ERR_USERSDONTMATCH, "Can't change mode for other users"));
	}
}


namespace IrcReply
{
	std::string welcome(const std::string &userPrefix)
	{
		return ("Welcome to the Internet Relay Network " + userPrefix);
	}

	std::string yourhost(const std::string &servername, const std::string &version)
	{
		return ("Your host is " + servername + ", running version " + version);
	}

	std::string created(const std::string &date)
	{
		return ("This server was created " + date);
	}

	std::string myinfo(const std::string &servername, const std::string &version, const std::string &availableUserModes, const std::string &availableChannelModes)
	{
		return (servername + ' ' + version + ' ' + availableUserModes + ' ' + availableChannelModes);
	}

	std::string motdstart(const std::string &servername)
	{
		return ("- " + servername + " Message of the day - ");
	}

	std::string motd(const std::string &text)
	{
		return ("- " + text);
	}

	std::string endofmotd()
	{
		return ("End of MOTD command");
	}
}
