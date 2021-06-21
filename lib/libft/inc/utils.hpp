#pragma once
#include <sstream>
#include <vector>

namespace ft
{
	void fileToData(const std::string &filename, std::vector<std::string> &data, size_t sizeLine);

	bool match(const std::string &mask, const std::string &str);

	std::string toDate(const time_t t, const char *format);

	void toUpper(std::string &str);

	template<class T>
	inline std::string toString(const T &value)
	{
		std::stringstream stringbuf;
		stringbuf << value;
		return (stringbuf.str());
	}
} /* end of namespace ft */
