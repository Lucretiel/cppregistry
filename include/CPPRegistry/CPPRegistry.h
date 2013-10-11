#pragma once

#include <string>

//Thrown when calling a function that isn't in the registry
struct function_not_registered
{};

/////////////////////////////////////////////////////////////////////
// FUNCTION MACROS
/////////////////////////////////////////////////////////////////////

//Create named global function registry with given key, return, and argument types
#define BASIC_FUNCTION_REGISTRY(REGISTRY_NAME, KEY_TYPE, RETURN_TYPE, ...) \
	namespace registries {struct ##REGISTRY_NAME##_nametag {}; \
		typedef registry::GlobalFunctionRegistry< \
			##REGISTRY_NAME##_nametag, \
			KEY_TYPE, RETURN_TYPE, __VA_ARGS__> REGISTRY_NAME;}

//Create a named global function registry with given return and argument types, using std::string as the key type
#define FUNCTION_REGISTRY(REGISTRY_NAME, RETURN_TYPE, ...) \
	BASIC_FUNCTION_REGISTRY(REGISTRY_NAME, std::string, RETURN_TYPE, __VA_ARGS__)

//Register a function using a given key
#define REGISTER_FUNCTION(REGISTRY, FUNCTION, KEY) \
	namespace {const bool reg_##REGISTRY##_##FUNCTION##( \
		registries::REGISTRY::register_function((KEY), (&(FUNCTION))));}

//Register a function, using the function's name as the key
#define REGISTER_NAMED_FUNCTION(REGISTRY, FUNCTION) \
	REGISTER_FUNCTION(REGISTRY, FUNCTION, #FUNCTION)

//Retrieve a function by name
#define GET_FUNCTION(REGISTRY, KEY) \
	(registries::REGISTRY::get_function((KEY)))

//Call a function by name. May throw function_not_registered 
#define CALL_FUNCTION(REGISTRY, KEY, ...) \
	(registries::REGISTRY::call_function((KEY), __VA_ARGS__))

//Get all the registered function names
#define GET_REGISTERED_FUNCTIONS(REGISTRY) \
	(registries::REGISTRY::get_registered_functions())

//Header for declaring, registering, and defining a function, all in one line.
//Usage: REGISTERED_FUNCTION(Registry, function_name, key, type1 argument1, ...) {function body}
#define REGISTERED_FUNCTION(REGISTRY, FUNCTION, KEY, ...) \
	registries::REGISTRY::return_type FUNCTION(__VA_ARGS__); \
	REGISTER_FUNCTION(REGISTRY, FUNCTION, KEY); \
	registries::REGISTRY::return_type FUNCTION(__VA_ARGS__)

//Header for creating preregistered functions with the function name as the key
#define REGISTERED_NAMED_FUNCTION(REGISTRY, FUNCTION, ...) \
	REGISTERED_FUNCTION(REGISTRY, FUNCTION, #FUNCTION, __VA_ARGS__)

/////////////////////////////////////////////////////////////////////
// MEMBER FUNCTION MACROS
/////////////////////////////////////////////////////////////////////

//Most of these match up with the standalone function macros above

#define BASIC_MEMBER_FUNCTION_REGISTRY(REGISTRY_NAME, KEY_TYPE, RETURN_TYPE, OBJECT_TYPE, ...) \
	namespace registries {struct ##REGISTRY_NAME##_nametag {}; \
		typedef registry::GlobalMemberFunctionRegistry< \
			##REGISTRY_NAME##_nametag, \
			KEY_TYPE, RETURN_TYPE, OBJECT_TYPE, __VA_ARGS__> REGISTRY_NAME;}

#define MEMBER_FUNCTION_REGISTRY(REGISTRY_NAME, RETURN_TYPE, OBJECT_TYPE, ...) \
	BASIC_MEMBER_FUNCTION_REGISTRY(REGISTRY_NAME, std::string, RETURN_TYPE, OBJECT_TYPE, __VA_ARGS__)

#define REGISTER_MEMBER_FUNCTION(REGISTRY, FUNCTION, KEY) \
	namespace { const bool reg_##REGISTRY##_##FUNCTION( \
		registries::REGISTRY::register_function((KEY), (&(REGISTRY::object_type::FUNCTION))));}

#define REGISTER_NAMED_MEMBER_FUNCTION(REGISTRY, FUNCTION) \
	REGISTER_MEMBER_FUNCTION(REGISTRY, FUNCTION, #FUNCTION)

#define GET_MEMBER_FUNCTION(REGISTRY, KEY) \
	(registries::REGISTRY::get_function((KEY)))

#define CALL_MEMBER_FUNCTION(REGISTRY, KEY, OBJECT, ...) \
	(registries::REGISTRY::call_function((OBJECT), (KEY), __VA_ARGS__))

#define GET_REGISTERED_MEMBER_FUNCTIONS(REGISTRY) \
	(registries::REGISTRY::get_registered_functions())


/////////////////////////////////////////////////////////////////////
// TYPE MACROS
/////////////////////////////////////////////////////////////////////

#define BASIC_TYPE_REGISTRY(REGISTRY_NAME, KEY_TYPE, BASE_TYPE) \
	namespace registries {struct ##REGISTRY_NAME##_nametag {}; \
		typedef registry::GlobalTypeRegistry< \
			##REGISTRY_NAME##_nametag, \
			KEY_TYPE, BASE_TYPE> REGISTRY_NAME;}

#define TYPE_REGISTRY(REGISTRY_NAME, BASE_TYPE) \
	BASIC_TYPE_REGISTRY(REGISTRY_NAME, std::string, BASE_TYPE)

#define REGISTER_TYPE(REGISTRY, TYPE, KEY) \
	namespace {const bool reg_##REGISTRY##_##TYPE( \
		registries::REGISTRY::register_type<TYPE>((KEY)));}

#define REGISTER_NAMED_TYPE(REGISTRY, TYPE) \
	REGISTERED_TYPE(REGISTRY, TYPE, #TYPE)

#define MAKE_TYPE(REGISTRY, KEY) \
	(registries::REGISTRY::make_type((KEY)))

#define GET_REGISTERED_TYPES(REGISTRY) \
	(registries::REGISTRY::get_registered_types())

//Note: This header does NOT explicitly derive TYPE from BASE. You'll have to do:
//REGISTERED_TYPE(Registry, Derived, "der"): public Base {class body}
#define REGISTERED_TYPE(REGISTRY, TYPE, KEY) \
	class TYPE; \
	REGISTER_TYPE(REGISTRY, TYPE, KEY); \
	class TYPE

#define REGISTERED_NAMED_TYPE(REGISTRY, TYPE) \
	REGISTERED_TYPE(REGISTRY, TYPE, #TYPE)

#include <CPPRegistry/CPPRegistry-impl.h>