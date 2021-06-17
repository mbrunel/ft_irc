#include "IrcServer.hpp"
#include "libft.hpp"

static const char *info[] = {
"ircserv",
"",
"Based on RFC 2810, 2811, 2812, 2813, 7194",
"",
"This program is free software; you can redistribute it and/or",
"modify it under the terms of the GNU General Public License as",
"published by the Free Software Foundation; either version 3",
"or (at your option) any later version.",
"",
"/!\\ - MUST NEVER EVER BE PUT ONLINE",
"",
"Authors:",
"- Adrien Soursou",
"- Matthias Brunel",
"- Mattéo Papin",
"",
NULL };

int IrcServer::info(User &u, const IRC::Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IRC::Error::notregistered()));
	if (m.params().size() && !ft::match(m.params()[0], config.servername))
		return (writeNum(u, IRC::Error::nosuchserver(m.params()[0])));

	for (size_t i = 0; ::info[i]; ++i)
		writeNum(u, IRC::Reply::info(::info[i]));
	std::string timestamp = "Compilation date: ";
	writeNum(u, IRC::Reply::info(timestamp + __DATE__ " at " + __TIME__));
	return writeNum(u, IRC::Reply::endofinfo());
}
