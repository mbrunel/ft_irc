#include <cstring>
#include "Parser.hpp"
#include "libft.hpp"

namespace IRC
{

namespace Parser
{
	bool asChannel(Context &c, std::string &s)
	{
		c.resetDistance();
		if (!strchr("#+", *c))
			return (false);
		while ((++c).distance() < 50 && *c && *c != ',' && *c != '\b');
		if (c.distance() < 2)
			return (false);
		s = c.extract();
		return (true);
	}

	bool asCommand(Context &c, std::string &s)
	{
		c.resetDistance();
		if (std::isalpha(*c))
			while (std::isalpha(*(++c)));
		else if (!std::isdigit(*c))
			return (false);
		else
			while ((++c).distance() < 3)
				if (!std::isdigit(*c))
					return (false);
		ft::toUpper(s = c.extract().c_str());
		return (true);
	}

	bool asHost(Context &c, std::string &s)
	{
		Context ipv4(c), ipv6(c);

		if (isIPv4(ipv4))
			c = ipv4;
		else if (isIPv6(ipv6))
			c = ipv6;
		else
			return (asHostname(c, s));
		s = c.extract();
		return (c.distance() > 0);
	}

	bool asHostname(Context &c, std::string &s)
	{
		c.resetDistance();
		if (!isShortname(c))
			return (false);
		while (*c == '.')
			if (!isShortname(++c))
				return (false);
		s = c.extract();
		return (true);
	}

	bool asNickname(Context &c, std::string &s)
	{
		if (!std::isalpha(*c) && !c.isSpecial())
			return (false);
		c.resetDistance();
		do
			++c;
		while (*c && (std::isalnum(*c) || c.isSpecial() || *c == '-') &&
		c.distance() < 9);
		s = c.extract();
		return (true);
	}

	bool asUser(Context &c, std::string &s)
	{
		c.resetDistance();
		while (!c.isSpace() && *c != '@')
			++c;
		s = c.extract();
		return (c.distance() > 0);
	}

	bool isNumber(Context &c, size_t min, size_t max)
	{
		size_t n = 0;

		if (!std::isdigit(*c) || (*c == '0' && std::isdigit(*(++c))))
			return (false);
		while (std::isdigit(*c))
		{
			n = 10 * n + *c - '0';
			if (n > max)
				return (false);
			++c;
		}
		return (n >= min);
	}

	bool isXNumber(Context &c, size_t min, size_t max)
	{
		size_t n = 0;

		if (!std::isxdigit(*c) || (*c == '0' && std::isxdigit(*(++c))))
			return (false);
		while (std::isxdigit(*c))
		{
			n *= 16;
			n += *c - (std::isdigit(*c) ? *c - '0' : std::tolower(*c) - 'a');
			if (n > max)
				return (false);
			++c;
		}
		return (n >= min);
	}

	bool isIPv4(Context &c)
	{
		c.resetDistance();
		if (!isNumber(c, 0, 255))
			return (false);
		for (size_t i = 0; i < 3; ++i)
			if (*c != '.' || !isNumber(++c, 0, 255))
				return (false);
		return (true);
	}

	bool isIPv6(Context &c)
	{
		c.resetDistance();
		if (!isXNumber(c, 0, 0xffff))
			return (false);
		for (size_t i = 0; i < 7; ++i)
			if (*c != '.' || !isXNumber(++c, 0, 0xffff))
				return (false);
		return (true);
	}

	bool isShortname(Context &c)
	{
		size_t	prevDist = c.distance();
		char	last;

		if (std::isalnum(*c))
			do
				last = *c;
			while (std::isalnum(*(++c)) || *c == '-' || *c == '_');
		return (c.distance() - prevDist > 1 && std::isalnum(last));
	}
}

} /* end of namespace IRC */
