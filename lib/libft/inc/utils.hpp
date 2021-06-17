#pragma once
#include <sstream>
#include <vector>

namespace ft
{
	void file_to_data(const std::string &filename, std::vector<std::string> &data);

	bool match(const std::string &mask, const std::string &str);

	std::string to_date(const time_t t, const char *format);

	void to_upper(std::string &str);

	template<class T>
	inline std::string to_string(const T &value)
	{
		std::stringstream stringbuf;
		stringbuf << value;
		return (stringbuf.str());
	}
} /* end of namespace ft */
