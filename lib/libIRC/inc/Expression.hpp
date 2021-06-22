#pragma once
#include "Context.hpp"

namespace IRC
{

/**
 * A generic expression.
 */
class Expression
{
public:
	Expression();
	virtual ~Expression();

	/**
	 * \return true is this expression is valid
	*/
	bool isValid() const;

	/**
	 * This function is called by the main expression and should not be used directly.
	 * \param c The context to use
	 * \return true if this expression is valid
	*/
	virtual bool interpret(Context &c) = 0;

protected:
	bool accept();
	bool reject();

private:
	bool _isValid;
};

} /* end of namespace IRC */
