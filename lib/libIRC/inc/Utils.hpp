#pragma once
#include <sstream>
#include <string>

namespace Utils
{
	bool match(const std::string &mask, const std::string &str);

	std::string to_date(const time_t t, const char *format);

	template<class T>
	inline std::string to_string(const T &value)
	{
		std::stringstream stringbuf;
		stringbuf << value;
		return (stringbuf.str());
	}
}
