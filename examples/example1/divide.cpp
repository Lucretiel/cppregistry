#include "operator.h"

/*
 *  Just like in multiply, but using the function name as the registered name.
 */
int divide(int a, int b)
{
	return a / b;
}

REGISTER_NAMED_FUNCTION(Operators, divide);