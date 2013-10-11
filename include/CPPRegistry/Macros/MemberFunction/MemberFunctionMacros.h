#pragma once

#include <CPPRegistry/MemberFunction/GlobalMemberFunctionRegistry.h>

#define BASIC_MEMBER_FUNCTION_REGISTRY(REGISTRY_NAME, KEY_TYPE, RETURN_TYPE, OBJECT_TYPE, ...) \
	namespace registries {struct _##REGISTRY_NAME##_nametag {}; \
		typedef registry::NamedGlobalMemberFunctionRegistry< \
			_##REGISTRY_NAME##_nametag, \
			KEY_TYPE, RETURN_TYPE, OBJECT_TYPE, __VA_ARGS__> REGISTRY_NAME;}

#define REGISTER_MEMBER_FUNCTION(REGISTRY, FUNCTION, KEY) \
	namespace { const bool _reg_##REGISTRY##_##FUNCTION( \
		registries::REGISTRY::register_function((KEY), (&(REGISTRY::object_type::FUNCTION))));}

#define GET_MEMBER_FUNCTION(REGISTRY, KEY) \
	(registries::REGISTRY::get_function((KEY)))

#define CALL_MEMBER_FUNCTION(REGISTRY, KEY, OBJECT, ...) \
	(registries::REGISTRY::call_function((OBJECT), (KEY), __VA_ARGS__))

#define GET_REGISTERED_MEMBER_FUNCTIONS(REGISTRY) \
	(registries::REGISTRY::get_registered_functions())

//TODO: preregistration header