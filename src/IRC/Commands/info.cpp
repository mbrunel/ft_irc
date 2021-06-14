#include "IrcServer.hpp"
#include "ft.hpp"

static const char *info[] = {
"IRC --",
"Based on RFC 2810, 2811, 2812, 2813, 7194",
"",
"This program is free software; you can redistribute it and/or",
"modify it under the terms of the GNU General Public License as",
"published by the Free Software Foundation; either version 3",
"or (at your option) any later version.",
"",
"MUST NEVER EVER BE PUT ONLINE",
NULL };

int IrcServer::info(User &u, const Message &m)
{
	if (!u.isRegistered())
		return (writeNum(u, IrcError::notregistered()));
	if (m.params().size())
		if (!ft::match(m.params()[0].mask(), config.servername))
			return (writeNum(u, IrcError::nosuchserver(m.params()[0])));

	for (size_t i = 0; ::info[i]; ++i)
		writeNum(u, IrcReply::info(::info[i]));
	std::string timestamp = "Compilation date : ";
	writeNum(u, IrcReply::info(timestamp + __DATE__ " at " + __TIME__));
	return writeNum(u, IrcReply::endofinfo());
}
