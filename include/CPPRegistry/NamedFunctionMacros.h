#pragma once

#include <string>
#include <CPPRegistry\FunctionMarcos.h>

//Create a named global function registry with given return and argument types, using std::string as the key type
#define FUNCTION_REGISTRY(REGISTRY_NAME, RETURN_TYPE, ...) \
	BASIC_FUNCTION_REGISTRY(REGISTRY_NAME, std::string, RETURN_TYPE, __VA_ARGS__)

//Globally register a function, using the function's name as the key
#define REGISTER_NAMED_FUNCTION(REGISTRY, FUNCTION) \
	REGISTER_FUNCTION(REGISTRY, FUNCTION, #FUNCTION)

//Header for creating preregistered functions
#define REGISTERED_NAMED_FUNCTION(REGISTRY, FUNCTION, ...) \
	REGISTERED_FUNCTION(REGISTRY, FUNCTION, #FUNCTION, __VA_ARGS__)