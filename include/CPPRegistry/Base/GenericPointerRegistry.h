#pragma once

#include <CPPRegistry/Base/GenericRegistry.h>

namespace registry
{

template<class Key, class Value>
class GenericPointerRegistry
{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef Value* value_ptr_type;

private:
	typedef GenericRegistry<Key, Value*> registry_type;
	registry_type m_registry;

public:
	typedef typename registry_type::keys_type keys_type;

	void register_pointer(const Key& key, Value* value)
	{
		m_registry.register_item(key, value);
	}

	const Value* get_pointer(const Key& key) const
	{
		const value_ptr* result = m_registry.get_item(key);
		return result ? *result : nullptr;
	}

	Value* get_pointer(const Key& key) const
	{
		value_ptr* result = m_registry.get_item(key);
		return result ? *result : nullptr;
	}

	const keys_type& get_registered_pointers() const
	{
		return m_registry.get_registered_items();
	}
};

}