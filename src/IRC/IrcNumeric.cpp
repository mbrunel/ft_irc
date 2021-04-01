/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcNumeric.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/01 16:04:19 by asoursou          #+#    #+#             */
/*   Updated: 2021/04/01 16:10:13 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcNumeric.hpp"

namespace IrcError
{
	std::string nosuchnick(const std::string &hostname, const std::string &nickname) {
		return (':' + hostname + " 401 " + nickname + " :No such nick/channel");
	}
	std::string nosuchserver(const std::string &hostname, const std::string &servername) {
		return (':' + hostname + " 402 " + servername + " :No such server");
	}
	std::string nosuchchannel(const std::string &hostname, const std::string &channelname) {
		return (':' + hostname + " 403 " + channelname + " :No such channel");
	}
	std::string cannotsendtochan(const std::string &hostname, const std::string &channelname) {
		return (':' + hostname + " 404 " + channelname + " :Cannot send to channel");
	}
	std::string toomanychannels(const std::string &hostname, const std::string &channelname) {
		return (':' + hostname + " 405 " + channelname + " :You have joined too many channels");
	}
	std::string wasnosuchnick(const std::string &hostname, const std::string &nickname) {
		return (':' + hostname + " 406 " + nickname + " :There was no such nickname");
	}
	std::string toomanytargets(const std::string &hostname, const std::string &target) {
		return (':' + hostname + " 407 " + target + " :Duplicate recipients. No message delivered");
	}
	std::string noorigin(const std::string &hostname) {
		return (':' + hostname + " 409 :No origin specified");
	}
	std::string norecipient(const std::string &hostname, const std::string &command) {
		return (':' + hostname + " 411 :No recipient given " + command);
	}
	std::string notexttosend(const std::string &hostname) {
		return (':' + hostname + " 412 :No text to send");
	}
	std::string notoplevel(const std::string &hostname, const std::string &mask) {
		return (':' + hostname + " 413 " + mask + " :No top level domain specified");
	}
	std::string wildtoplevel(const std::string &hostname, const std::string &mask) {
		return (':' + hostname + " 414 " + mask + " :Wildcard in top level domain");
	}
	std::string unknowncommand(const std::string &hostname, const std::string &command) {
		return (':' + hostname + " 421 " + command + " :Unknown command");
	}
	std::string nomotd(const std::string &hostname) {
		return (':' + hostname + " 422 :MOTD file is missing");
	}
	std::string noadmininfo(const std::string &hostname) {
		return (':' + hostname + " 423 :No administrative info available");
	}
	std::string fillerror(const std::string &hostname, const std::string &file_op, const std::string &file) {
		return (':' + hostname + " 424 :File error doing " + file_op + " on " + file);
	}
	std::string nonicknamegiven(const std::string &hostname) {
		return (':' + hostname + " 431 :No nickame given");
	}
	std::string erroneusnickname(const std::string &hostname, const std::string &nickname) {
		return (':' + hostname + " 432 " + nickname + " :Erroneus nickname");
	}
	std::string nicknameinuse(const std::string &hostname, const std::string &nickname) {
		return (':' + hostname + " 433 " + nickname + " :Nickname is already in use");
	}
	std::string nickcollision(const std::string &hostname, const std::string &nickname) {
		return (':' + hostname + " 436 " + nickname + " :Nickname collision KILL");
	}
	std::string usernotinchannel(const std::string &hostname, const std::string &nickname, const std::string &channel) {
		return (':' + hostname + " 441 " + nickname + " " + channel + " :They aren't on that channel");
	}
	std::string notonchannel(const std::string &hostname, const std::string &channel) {
		return (':' + hostname + " 442 " + channel + " :You're not on that channel");
	}
	std::string useronchannel(const std::string &hostname, const std::string &nickname, const std::string &channel) {
		return (':' + hostname + " 443 " + nickname + " " + channel + " :is already on channel");
	}
	std::string nologin(const std::string &hostname, const std::string &user) {
		return (':' + hostname + " 444 " + user + " :User not logged in");
	}
	std::string summondisabled(const std::string &hostname) {
		return (':' + hostname + " 445 :SUMMON has been disabled");
	}
	std::string usersdisabled(const std::string &hostname) {
		return (':' + hostname + " 446 :USERS has been disabled");
	}
	std::string notregistered(const std::string &hostname) {
		return (':' + hostname + " 451 :You have not registered");
	}
	std::string needmoreparams(const std::string &hostname, const std::string &command) {
		return (':' + hostname + " 461 " + command + " :Not enough parameters");
	}
	std::string alreadyregistred(const std::string &hostname) {
		return (':' + hostname + " 462 :You may not reregister");
	}
	std::string nopermforhost(const std::string &hostname) {
		return (':' + hostname + " 463 :Your host is not among the privileged");
	}
	std::string passwdmissmatch(const std::string &hostname) {
		return (':' + hostname + " 464 :Passwordincorrect");
	}
	std::string yourebannedcreep(const std::string &hostname) {
		return (':' + hostname + " 465 :You're banned from this server");
	}
	std::string keyset(const std::string &hostname, const std::string &channel) {
		return (':' + hostname + " 467 " + channel + " :Channel key already set");
	}
	std::string channelisfull(const std::string &hostname, const std::string &channel) {
		return (':' + hostname + " 471 " + channel + " :Channel is full");
	}
	std::string unknownmode(const std::string &hostname, const std::string &c) {
		return (':' + hostname + " 472 " + c + " :is unknown mode char to me");
	}
	std::string inviteonlychan(const std::string &hostname, const std::string &channel) {
		return (':' + hostname + " 473 " + channel + " :Cannot join channel (+i)");
	}
	std::string bannedfromchan(const std::string &hostname, const std::string &channel) {
		return (':' + hostname + " 474 " + channel + " :Cannot join channel (+b)");
	}
	std::string badchannelkey(const std::string &hostname, const std::string &channel) {
		return (':' + hostname + " 475 " + channel + " :Cannot join channel (+k)");
	}
	std::string noprivileges(const std::string &hostname) {
		return (':' + hostname + " 481 :Permission Denied- You're not an IRC operator");
	}
	std::string chanoprisneeded(const std::string &hostname, const std::string &channel) {
		return (':' + hostname + " 482 " + channel + " :You're not channel operator");
	}
	std::string cantkillserver(const std::string &hostname) {
		return (':' + hostname + " 483 :You can't kill a server");
	}
	std::string nooperhost(const std::string &hostname) {
		return (':' + hostname + " 491 :No O-lines for your host");
	}
	std::string umodeunknownflag(const std::string &hostname) {
		return (':' + hostname + " 501 :Unknown MODE flag");
	}
	std::string usersdontmatch(const std::string &hostname) {
		return (':' + hostname + " 502 :Can't change mode for other users");
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
