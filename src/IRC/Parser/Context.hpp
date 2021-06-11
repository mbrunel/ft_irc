#pragma once
#include <string>

class Context
{
public:
	Context(const std::string &data);
	virtual ~Context();

	size_t		distance() const;
	std::string	extract() const;
	bool		isNospcrlfcl() const;
	bool		isSpace() const;
	bool		isSpecial() const;
	void		resetDistance();
	char		operator*() const;
	Context		&operator++();
	Context		operator++(int);
	Context		&operator=(const Context &c);

private:
	typedef std::basic_string<char>::const_iterator	const_iterator;

	const std::string	&_data;
	const_iterator		_start;
	const_iterator		_i;
	size_t				_distance;
};
