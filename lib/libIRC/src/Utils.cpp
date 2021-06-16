#include "Utils.hpp"

#define TO_DATE_BUFSIZE	160

// std::string goes brrr
static bool match_internal(const char *mask, const char *str)
{
	const char m = *mask;
	const char c = *str;
	if (!m)
		return (!c);
	if (m == '\\') // escape character (only if next is a special character)
	{
		const char m1 = mask[1];
		if (m1 == '*' || m1 == '?')
			return (m1 == c ? match_internal(mask + 2, str + 1) : 0);
	}
	if (m == '*') // wildcard matching
	{
		if (match_internal(mask + 1, str))
			return (1);
		if (!c)
			return (0);
	}
	else
	{
		if (m == '?') // any character matching
		{
			if (!c)
				return (0);
		}
		else if (m != c) // else character must match
			return (0);
		++mask;
	}
	return (match_internal(mask, str + 1));
}

namespace Utils
{

void file_to_data(const std::string &filename, std::vector<std::string> &data)
{
	std::ifstream f(filename.c_str(), std::ios_base::in);
	std::string line;

	data.clear();
	while (std::getline(f, line))
	{
		if (line.size() > 80)
			line.resize(80);
		data.push_back(line);
	}
}

bool match(const std::string &mask, const std::string &str)
{
	return (match_internal(mask.c_str(), str.c_str()));
}

std::string to_date(const time_t t, const char *format)
{
	char buf[TO_DATE_BUFSIZE];

	strftime(buf, sizeof(buf), format, localtime(&t));
	return (buf);
}

void to_upper(std::string &str)
{
	for (std::string::iterator c = str.begin(); c != str.end(); ++c) *c = toupper(*c);
}

} /* end of namespace stdext */
