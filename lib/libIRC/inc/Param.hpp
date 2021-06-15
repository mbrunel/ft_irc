#pragma once
#include <vector>
#include "Context.hpp"

namespace IRC
{

class Param : public std::string
{
public:
	Param();
	Param(const std::string &value);
	virtual ~Param();

	bool				isChannel() const;
	bool				isMask() const;
	bool				isNickname() const;
	bool				isKey() const;
	std::string			mask() const;
	std::vector<Param>	split(char delimiter = ',') const;

private:
	bool	check(bool (*parsing_func)(Context &, std::string &)) const;
};
} /* end of namespace IRC */
