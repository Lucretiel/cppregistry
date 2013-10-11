#pragma once

#include <CPPRegistry/Type/GlobalTypeRegistry.h>

#define BASIC_TYPE_REGISTRY(REGISTRY_NAME, KEY_TYPE, BASE_TYPE) \
	namespace registries {struct _##REGISTRY_NAME##_nametag {}; \
		typedef registry::NamedGlobalTypeRegistry< \
			__##REGISTRY_NAME##_nametag, \
			KEY_TYPE, BASE_TYPE> REGISTRY_NAME;}

#define REGISTER_TYPE(REGISTRY, TYPE, KEY) \
	namespace {const bool _reg_##REGISTRY##_##TYPE( \
		registries::REGISTRY::register_type<TYPE>((KEY)));}

#define MAKE_TYPE(REGISTRY, KEY) \
	(registries::REGISTRY::make_type((KEY)))

#define GET_REGISTERED_TYPES(REGISTRY) \
	(registries::REGISTRY::get_registered_types())

//Note: This header does NOT explicitly derive TYPE from BASE.
#define REGISTERED_TYPE(REGISTRY, TYPE, KEY) \
	class TYPE; \
	REGISTER_TYPE(REGISTRY, TYPE, KEY); \
	class TYPE