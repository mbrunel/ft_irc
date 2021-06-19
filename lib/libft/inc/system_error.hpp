#pragma once
#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace ft
{
	class system_error : public std::runtime_error
	{
	public:
		system_error(const std::string &what_arg) :
		std::runtime_error(what_arg + ": " + std::strerror(errno)), _code(errno)
		{}

		system_error(const int code, const std::string &what_arg) :
		std::runtime_error(what_arg + ": " + std::strerror(code)), _code(code)
		{}

		virtual ~system_error() throw()
		{}

		int code() const throw()
		{
			return (_code);
		}

	private:
		const int _code;
	};
} /* end of namespace ft */
