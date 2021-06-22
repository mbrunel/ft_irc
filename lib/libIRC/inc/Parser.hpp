#pragma once
#include "Context.hpp"

namespace IRC
{

/**
 * Contains some grammar rules from RFC 2812
 */
namespace Parser
{
	bool asChannel(Context &c, std::string &s);
	bool asCommand(Context &c, std::string &s);
	bool asHost(Context &c, std::string &s);
	bool asHostname(Context &c, std::string &s);
	bool asNickname(Context &c, std::string &s);
	bool asUser(Context &c, std::string &s);
	bool isNumber(Context &c, size_t min, size_t max);
	bool isXNumber(Context &c, size_t min, size_t max);
	bool isIPv4(Context &c);
	bool isIPv6(Context &c);
	bool isShortname(Context &c);
}

} /* end of namespace IRC */
