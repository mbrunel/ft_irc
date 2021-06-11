#include <cstring>
#include "Context.hpp"

Context::Context(const std::string &data) :
_data(data), _start(data.begin()), _i(_start), _distance(0)
{}

Context::~Context()
{}

size_t Context::distance() const
{
	return (_distance);
}

std::string Context::extract() const
{
	return (std::string(_start, _i));
}

bool Context::isNospcrlfcl() const
{
	char c = **this;

	return (c && !isSpace() && c != ':');
}

bool Context::isSpace() const
{
	return (!!std::strchr("\r\n ", **this));
}

bool Context::isSpecial() const
{
	return (!!std::strchr("[]\\`_^{|}", **this));
}

void Context::resetDistance()
{
	_start = _i;
	_distance = 0;
}

char Context::operator*() const
{
	return (_i == _data.end() ? '\0' : *_i);
}

Context &Context::operator++()
{
	if (_i != _data.end())
	{
		++_i;
		++_distance;
	}
	return (*this);
}

Context Context::operator++(int)
{
	Context copy(*this);

	++*this;
	return (copy);
}

Context &Context::operator=(const Context &c)
{
	if (&_data == &c._data)
	{
		_start = c._start;
		_i = c._i;
		_distance = c._distance;
	}
	return (*this);
}
