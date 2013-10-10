#pragma once

#include <CPPRegistry\GlobalFunctionRegistry.h>

//Create named global function registry with given key, return, and argument types
#define BASIC_FUNCTION_REGISTRY(REGISTRY_NAME, KEY_TYPE, RETURN_TYPE, ...) \
	namespace registries {struct _##REGISTRY_NAME##_nametag {}; \
		typedef registry::NamedGlobalFunctionRegistry< \
			_##REGISTRY_NAME##_nametag, \
			KEY_TYPE, RETURN_TYPE, __VA_ARGS__> REGISTRY_NAME;}

//Globally register a function using a given key
#define REGISTER_FUNCTION(REGISTRY, FUNCTION, KEY) \
	namespace {const bool _reg_##REGISTRY##_##FUNCTION( \
		registries::REGISTRY::register_function((KEY), (&(FUNCTION))));}

//Retrieve a function by name
#define GET_FUNCTION(REGISTRY, KEY) \
	(registries::REGISTRY::get_function((KEY)))

//Call a function by name. May throw key_not_registered 
#define CALL_FUNCTION(REGISTRY, KEY, ...) \
	(registries::REGISTRY::call_function((KEY), __VA_ARGS__))

//Get all the registered function names
#define GET_REGISTERED_FUNCTIONS(REGISTRY) \
	(registries::REGISTRY::get_registered_functions())

//Header for declaring, registering, and defining a function, all in one line.
//Usage: REGISTERED_FUNCTION(Registry, function_name, key, type1 argument1, ...)
#define REGISTERED_FUNCTION(REGISTRY, FUNCTION, KEY, ...) \
	registries::REGISTRY::return_type FUNCTION(__VA_ARGS__); \
	REGISTER_FUNCTION(REGISTRY, FUNCTION, KEY); \
	registries::REGISTRY::return_type FUNCTION(__VA_ARGS__)