#pragma once
#include "Context.hpp"
#include "Utils.hpp"

namespace IRC
{

namespace Parser
{
	bool	asChannel(Context &o, std::string &s);
	bool	asCommand(Context &o, std::string &s);
	bool	asHost(Context &o, std::string &s);
	bool	asHostname(Context &o, std::string &s);
	bool	asNickname(Context &o, std::string &s);
	bool	asUser(Context &o, std::string &s);
	bool	isNumber(Context &o, size_t min, size_t max);
	bool	isXNumber(Context &o, size_t min, size_t max);
	bool	isIPv4(Context &o);
	bool	isIPv6(Context &o);
	bool	isShortname(Context &o);
}

} /* end of namespace IRC */
