#pragma once
#include <sstream>
#include <vector>

namespace ft
{
	/**
	 * Loads the content of a file into a vector of strings.
	 * \param filename The path to the file
	 * \param data A vector where the content will be stored
	 * \param sizeLine The maximum length of a line (infinite by default)
	 * \throw ft::systemError On I/O error
	*/
	void fileToData(const std::string &filename, std::vector<std::string> &data, size_t sizeLine = -1);

	/**
	 * Checks if a string matches a mask.
	 * Supported operators are match one (?), match many or wilcard (*) and
	 * escape (\\).
	 * \par Examples:
	 * - match("a*", "abc") -> true
	 * - match("a*", "bcd") -> false
	 * - match("x?z", "xyz") -> true
	 * - match("\*", "*") -> true
	 * \param mask The mask used for matching
	 * \param str The string to match
	 * \return true if a match is found
	*/
	bool match(const std::string &mask, const std::string &str) throw();

	/**
	 * Formats the given time into a date using the given format.
	 * This function is a wrapper of strftime and the format remains the same.
	 * \param t The time to format
	 * \param format The format to use
	 * \return A string which contains the formatted date
	*/
	std::string toDate(const time_t t, const char *format) throw();

	/**
	* Converts all lowercase characters in a string to uppercase.
	* \param str The string to capitalize
	*/
	void toUpper(std::string &str) throw();

	/**
	* Converts any type to a string.
	* \param value The value to convert
	* \return The result of the conversion
	*/
	template<class T>
	inline std::string toString(const T &value)
	{
		std::stringstream stringbuf;
		stringbuf << value;
		return (stringbuf.str());
	}
} /* end of namespace ft */
