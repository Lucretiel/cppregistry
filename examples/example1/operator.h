#pragma once

#include <CPPRegistry\Macros\Function\NamedFunctionMacros.h>

/*
 *  This header creates a global function registry called Operators, which
 *  registers functions that take 2 int arguments and return an int. Clients
 *  who want to use operators only have to #include this file in order to make
 *  use of functions that are registered to it. The registrations themselves
 *  are resolved automatically by the linker, and they occur just before main
 *  executes. Note that Operators is NOT an object, not even a static one.
 */

FUNCTION_REGISTRY(Operators, int, int, int);
