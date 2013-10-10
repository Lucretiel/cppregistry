#include "operator.h"

/*
 *  This example shows that there is nothing too complicated going on under
 *  the hood with add and subtract. Both those functions are just regular
 *  functions, registered with the REGISTER_FUNCTION macro
 */
int multiply(int a, int b)
{
	return a * b;
}

REGISTER_FUNCTION(Operators, multiply, "mult");