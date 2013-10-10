#include "operator.h"

/*
 *  Here's a look under the hood. You are welcome to use this style instead of
 *  the macros- they simply help reduce the verbosity.
 */
int modulus(int a, int b)
{
	return a % b;
}

/*
 *  This is what actually allows the automatic, linker-resolved function
 *  registration to work. The static register_function function returns a
 *  dummy bool value, which is used to statically initialize a dummy local
 *  bool value.
 */
const static bool reg(registries::Operators::register_function("modulus", &modulus));