#pragma once
#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace ft
{
	/**
	 * Implementation of std::system_error for C++98
	 */
	class systemError : public std::runtime_error
	{
	public:
		/**
		 * Constructs a new systemError using errno.
		 * \param what_arg The prefix used to describe the error
		 */
		systemError(const std::string &what_arg) :
		std::runtime_error(what_arg + ": " + std::strerror(errno)), _code(errno)
		{}

		/**
		 * Constructs a new systemError using an error code from errno.
		 * \param code The error code to use
		 * \param what_arg The prefix used to describe the error
		 */
		systemError(const int code, const std::string &what_arg) :
		std::runtime_error(what_arg + ": " + std::strerror(code)), _code(code)
		{}

		virtual ~systemError() throw()
		{}

		/**
		 * \return The error code
		 */
		int code() const throw()
		{
			return (_code);
		}

	private:
		const int _code;
	};
} /* end of namespace ft */
