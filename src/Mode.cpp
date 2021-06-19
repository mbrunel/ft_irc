#include "Mode.hpp"

Mode::Mode(unsigned mask) :
_flags(mask)
{}

Mode::~Mode()
{}

bool Mode::areSet(unsigned mask) const
{
	return (!((_flags & mask) ^ mask));
}

unsigned Mode::flags() const
{
	return (_flags);
}

bool Mode::isSet(unsigned mask) const
{
	return (!!(_flags & mask));
}

void Mode::set(unsigned mask)
{
	_flags |= mask;
}

void Mode::unset(unsigned mask)
{
	_flags &= ~mask;
}
