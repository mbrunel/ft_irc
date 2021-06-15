#include "Expression.hpp"

namespace IRC
{

Expression::Expression() : _isValid(false)
{}

Expression::~Expression()
{}

bool Expression::isValid() const
{
	return (_isValid);
}

bool Expression::accept()
{
	return (_isValid = true);
}

bool Expression::reject()
{
	return (_isValid = false);
}

} /* end of namespace IRC */
