#include "operator.h"

/*
 *  This is the simplest example. The macro REGISTERED_NAMED_FUNCTION
 *  automatically defines the function add and registers it to the Operators
 *  registry. Notice again that there is need for an add.h.
 */
REGISTERED_NAMED_FUNCTION(Operators, add, int a, int b)
{
	return a + b;
}
