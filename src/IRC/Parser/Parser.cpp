#include <cstring>
#include "Parser.hpp"

namespace Parser
{
	bool asChannel(Context &o, std::string &s)
	{
		o.resetDistance();
		if (!strchr("#&+", *o))
			return (false);
		while ((++o).distance() < 50 && *o && *o != ',' && *o != '\b');
		if (o.distance() < 2)
			return (false);
		s = o.extract();
		return (true);
	}

	bool asCommand(Context &o, std::string &s)
	{
		o.resetDistance();
		if (std::isalpha(*o))
			while (std::isalpha(*(++o)));
		else if (!std::isdigit(*o))
			return (false);
		else
			while ((++o).distance() < 3)
				if (!std::isdigit(*o))
					return (false);
		s = o.extract();
		return (true);
	}

	bool asHost(Context &o, std::string &s)
	{
		Context ipv4(o), ipv6(o);

		if (isIPv4(ipv4))
			o = ipv4;
		else if (isIPv6(ipv6))
			o = ipv6;
		else
			return (asHostname(o, s));
		s = o.extract();
		return (o.distance() > 0);
	}

	bool asHostname(Context &o, std::string &s)
	{
		o.resetDistance();
		if (!isShortname(o))
			return (false);
		while (*o == '.')
			if (!isShortname(++o))
				return (false);
		s = o.extract();
		return (true);
	}

	bool asNickname(Context &o, std::string &s)
	{
		if (!std::isalpha(*o) && !o.isSpecial())
			return (false);
		o.resetDistance();
		do
			++o;
		while (*o && (std::isalnum(*o) || o.isSpecial() || *o == '-') &&
		o.distance() < 9);
		s = o.extract();
		return (true);
	}

	bool asUser(Context &o, std::string &s)
	{
		o.resetDistance();
		while (!o.isSpace() && *o != '@')
			++o;
		s = o.extract();
		return (o.distance() > 0);
	}

	bool isNumber(Context &o, size_t min, size_t max)
	{
		size_t n = 0;

		if (!std::isdigit(*o) || (*o == '0' && std::isdigit(*(++o))))
			return (false);
		while (std::isdigit(*o))
		{
			n = 10 * n + *o - '0';
			if (n > max)
				return (false);
			++o;
		}
		return (n >= min);
	}

	bool isXNumber(Context &o, size_t min, size_t max)
	{
		size_t n = 0;

		if (!std::isxdigit(*o) || (*o == '0' && std::isxdigit(*(++o))))
			return (false);
		while (std::isxdigit(*o))
		{
			n *= 16;
			n += *o - (std::isdigit(*o) ? *o - '0' : std::tolower(*o) - 'a');
			if (n > max)
				return (false);
			++o;
		}
		return (n >= min);
	}

	bool isIPv4(Context &o)
	{
		o.resetDistance();
		if (!isNumber(o, 0, 255))
			return (false);
		for (size_t i = 0; i < 3; ++i)
			if (*o != '.' || !isNumber(++o, 0, 255))
				return (false);
		return (true);
	}

	bool isIPv6(Context &o)
	{
		o.resetDistance();
		if (!isXNumber(o, 0, 0xffff))
			return (false);
		for (size_t i = 0; i < 7; ++i)
			if (*o != '.' || !isXNumber(++o, 0, 0xffff))
				return (false);
		return (true);
	}

	bool isShortname(Context &o)
	{
		size_t	prevDist = o.distance();
		char	last;

		if (std::isalnum(*o))
			do
				last = *o;
			while (std::isalnum(*(++o)) || *o == '-');
		return (o.distance() - prevDist > 1 && std::isalnum(last));
	}
}
