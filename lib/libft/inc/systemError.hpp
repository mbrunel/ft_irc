#pragma once
#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace ft
{
	class systemError : public std::runtime_error
	{
	public:
		systemError(const std::string &what_arg) :
		std::runtime_error(what_arg + ": " + std::strerror(errno)), _code(errno)
		{}

		systemError(const int code, const std::string &what_arg) :
		std::runtime_error(what_arg + ": " + std::strerror(code)), _code(code)
		{}

		virtual ~systemError() throw()
		{}

		int code() const throw()
		{
			return (_code);
		}

	private:
		const int _code;
	};
} /* end of namespace ft */
