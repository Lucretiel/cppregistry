#pragma once

#include <CPPRegistry/Type/TypeRegistry.h>

namespace registry
{

template<class NameTag, class Key, class Base>
class NamedGlobalTypeRegistry
{
private:
	typedef TypeRegistry<Key, Base> registry_type;

	static registry_type& get_registry()
	{
		static registry_type m_registry;
		return m_registry;
	}

public:
	typedef Key key_type;
	typedef Base base_type;
	typedef Base* base_ptr;
	typedef typename registry_type::keys_type keys_type;

	//returns bool for static initializtion
	template<class Derived>
	static bool register_type(const Key& key)
	{
		get_registry().register_type<Derived>(key);
		return true;
	}

	static Base* make_type(const Key& key)
	{
		return get_registry().make_type(key);
	}

	static const keys_type& get_registered_type()
	{
		return get_registry().get_registered_types();
	}
};

}