#pragma once
#include "IrcServer.hpp"

class Command
{
public:
	Command(const std::string &value);
	virtual ~Command();

	bool				isChannel() const;
	bool				isNickname() const;
	bool				isKey() const;
	std::vector<Command>	split(char delimiter = ',') const;

private:
	bool	check(bool (*parsing_func)(Context &, std::string &)) const;
};

