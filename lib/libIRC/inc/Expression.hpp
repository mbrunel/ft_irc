#pragma once
#include "Context.hpp"

namespace IRC
{

class Expression
{
public:
	Expression();
	virtual ~Expression();

	bool			isValid() const;
	virtual bool	interpret(Context &o) = 0;

protected:
	bool	accept();
	bool	reject();

private:
	bool	_isValid;
};

} /* end of namespace IRC */
