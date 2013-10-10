#include "operator.h"

/*
 *  This is a slightly more elaborate example than add. It shows how you can
 *  give any name to any function.
 */
REGISTERED_FUNCTION(Operators, subtract, "sub", int a, int b)
{
	return a - b;
}