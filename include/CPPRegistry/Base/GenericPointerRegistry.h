#pragma once

#include <CPPRegistry/Base/GenericRegistry.h>

namespace registry
{

template<class Key, class Ptr>
class GenericPointerRegistry
{
private:
	typedef GenericRegistry<Key, Ptr> registry_type;
	registry_type m_registry;

public:
	typedef Key key_type;
	typedef Ptr ptr_type;
	typedef typename registry_type::keys_type keys_type;

	void register_pointer(const Key& key, Ptr value)
	{
		m_registry.register_item(key, value);
	}

	const Ptr get_pointer(const Key& key) const
	{
		auto result = m_registry.get_item(key);
		return result ? *result : nullptr;
	}

	Ptr get_pointer(const Key& key)
	{
		auto result = m_registry.get_item(key);
		return result ? *result : nullptr;
	}

	const keys_type& get_registered_pointers() const
	{
		return m_registry.get_registered_items();
	}
};

}