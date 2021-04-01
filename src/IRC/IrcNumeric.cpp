/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumeric.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbrunel <mbrunel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:37:07 by mbrunel           #+#    #+#             */
/*   Updated: 2021/04/01 16:37:09 by mbrunel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"

namespace IrcError
{
	std::string nosuchnick(const std::string &nickname) {
		return  (nickname + " :No such nick/channel");
	}
	std::string nosuchserver(const std::string &servername) {
		return  (servername + " :No such server");
	}
	std::string nosuchchannel(const std::string &channelname) {
		return  (channelname + " :No such channel");
	}
	std::string cannotsendtochan(const std::string &channelname) {
		return  (channelname + " :Cannot send to channel");
	}
	std::string toomanychannels(const std::string &channelname) {
		return  (channelname + " :You have joined too many channels");
	}
	std::string wasnosuchnick(const std::string &nickname) {
		return  (nickname + " :There was no such nickname");
	}
	std::string toomanytargets(const std::string &target) {
		return  (target + " :Duplicate recipients. No message delivered");
	}
	std::string noorigin() {
		return (" :No origin specified");
	}
	std::string norecipient(const std::string &command) {
		return (" :No recipient given " + command);
	}
	std::string notexttosend() {
		return (" :No text to send");
	}
	std::string notoplevel(const std::string &mask) {
		return  (mask + " :No top level domain specified");
	}
	std::string wildtoplevel(const std::string &mask) {
		return  (mask + " :Wildcard in top level domain");
	}
	std::string unknowncommand(const std::string &command) {
		return  (command + " :Unknown command");
	}
	std::string nomotd() {
		return (" :MOTD file is missing");
	}
	std::string noadmininfo() {
		return (" :No administrative info available");
	}
	std::string fillerror(const std::string &file_op, const std::string &file) {
		return ( " :File error doing " + file_op + " on " + file);
	}
	std::string nonicknamegiven() {
		return (" :No nickame given");
	}
	std::string erroneusnickname(const std::string &nickname) {
		return  (nickname + " :Erroneus nickname");
	}
	std::string nicknameinuse(const std::string &nickname) {
		return  (nickname + " :Nickname is already in use");
	}
	std::string nickcollision(const std::string &nickname) {
		return  (nickname + " :Nickname collision KILL");
	}
	std::string usernotinchannel(const std::string &nickname, const std::string &channel) {
		return  (nickname + " " + channel + " :They aren't on that channel");
	}
	std::string notonchannel(const std::string &channel) {
		return  (channel + " :You're not on that channel");
	}
	std::string useronchannel(const std::string &nickname, const std::string &channel) {
		return  (nickname + " " + channel + " :is already on channel");
	}
	std::string nologin(const std::string &user) {
		return  (user + " :User not logged in");
	}
	std::string summondisabled() {
		return (" :Summon has been disabled");
	}
	std::string usersdisabled() {
		return (" :USERS has been disabled");
	}
	std::string notregistered() {
		return (":You have not registered");
	}
	std::string needmoreparams(const std::string &command) {
		return  (command + " :Not enough parameters");
	}
	std::string alreadyregistred() {
		return (" :You may not reregister");
	}
	std::string nopermforhost() {
		return (" :Your host is not among the privileged");
	}
	std::string passwdmissmatch() {
		return (" :Password incorrect");
	}
	std::string yourebannedcreep() {
		return ( " :you're banned from this server");
	}
	std::string keyset(const std::string &channel) {
		return  (channel + " :Channel key already set");
	}
	std::string channelisfull(const std::string &channel) {
		return  (channel + " :Channel is full");
	}
	std::string unknownmode(const std::string &c) {
		return  (c + " :is unknown mode char to me");
	}
	std::string inviteonlychan(const std::string &channel) {
		return  (channel + " :Cannot join channel (+i)");
	}
	std::string bannedfromchan(const std::string &channel) {
		return  (channel + " :Cannot join channel (+b)");
	}
	std::string badchannelkey(const std::string &channel) {
		return  (channel + " :Cannot join channel (+k)");
	}
	std::string noprivileges() {
		return ( " :Permission Denied- You're not an IRC operator");
	}
	std::string chanoprisneeded(const std::string &channel) {
		return  (channel + " :You're not channel operator");
	}
	std::string cantkillserver() {
		return ( " :You can't kill a server");
	}
	std::string nooperhost() {
		return ( " :No O-lines for your host");
	}
	std::string umodeunknownflag() {
		return (" :Unknown MODE flag");
	}
	std::string usersdontmatch() {
		return (" :Can't change mode for other users");
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
