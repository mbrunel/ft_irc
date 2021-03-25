/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcError.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asoursou <asoursou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/27 14:00:28 by mbrunel           #+#    #+#             */
/*   Updated: 2021/03/25 15:47:07 by asoursou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

namespace IrcError
{
	std::string nosuchnick(const std::string &hostname, const std::string &nickname);
	std::string nosuchserver(const std::string &hostname, const std::string &servername);
	std::string nosuchchannel(const std::string &hostname, const std::string &channelname);
	std::string cannotsendtochan(const std::string &hostname, const std::string &channelname);
	std::string toomanychannels(const std::string &hostname, const std::string &channelname);
	std::string wasnosuchnick(const std::string &hostname, const std::string &nickname);
	std::string toomanytargets(const std::string &hostname, const std::string &target);
	std::string noorigin(const std::string &hostname);
	std::string norecipient(const std::string &hostname, const std::string &command);
	std::string notexttosend(const std::string &hostname);
	std::string notoplevel(const std::string &hostname, const std::string &mask);
	std::string wildtoplevel(const std::string &hostname, const std::string &mask);
	std::string unknowncommand(const std::string &hostname, const std::string &command);
	std::string nomotd(const std::string &hostname);
	std::string noadmininfo(const std::string &hostname);
	std::string fillerror(const std::string &hostname, const std::string &file_op, const std::string &file);
	std::string nonicknamegiven(const std::string &hostname);
	std::string erroneusnickname(const std::string &hostname, const std::string &nickname);
	std::string nicknameinuse(const std::string &hostname, const std::string &nickname);
	std::string nickcollision(const std::string &hostname, const std::string &nickname);
	std::string usernotinchannel(const std::string &hostname, const std::string &nickname, const std::string &channel);
	std::string notonchannel(const std::string &hostname, const std::string &channel);
	std::string useronchannel(const std::string &hostname, const std::string &nickname, const std::string &channel);
	std::string nologin(const std::string &hostname, const std::string &user);
	std::string summondisabled(const std::string &hostname);
	std::string usersdisabled(const std::string &hostname);
	std::string notregistered(const std::string &hostname);
	std::string needmoreparams(const std::string &hostname, const std::string &command);
	std::string alreadyregistred(const std::string &hostname);
	std::string nopermforhost(const std::string &hostname);
	std::string passwdmissmatch(const std::string &hostname);
	std::string yourebannedcreep(const std::string &hostname);
	std::string keyset(const std::string &hostname, const std::string &channel);
	std::string channelisfull(const std::string &hostname, const std::string &channel);
	std::string unknownmode(const std::string &hostname, const std::string &c);
	std::string inviteonlychan(const std::string &hostname, const std::string &channel);
	std::string bannedfromchan(const std::string &hostname, const std::string &channel);
	std::string badchannelkey(const std::string &hostname, const std::string &channel);
	std::string noprivileges(const std::string &hostname);
	std::string chanoprisneeded(const std::string &hostname, const std::string &channel);
	std::string cantkillserver(const std::string &hostname);
	std::string nooperhost(const std::string &hostname);
	std::string umodeunknownflag(const std::string &hostname);
	std::string usersdontmatch(const std::string &hostname);
}
