#pragma once

#include <unordered_map>
#include <unordered_set>

namespace registry
{

template<class Key, class Value>
class GenericRegistry
{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef std::unordered_set<key_type> keys_type;

private:
	typedef std::unordered_map<keys_type, value_type> registry_type;

	registry_type m_registry;
	keys_type keys;

public:
	template<class T>
	void register_item(const key_type& key, T&& value)
	{
		m_registry.emplace(key, std::forward<T>(value));
		keys.emplace(key);
	}

	value_type* get_item(const key_type& key) const
	{
		auto result = m_registry.find(key);
		return result != m_registry.end() ?
			&result->second : nullptr;
	}

	const keys_type& get_registered_items() const
	{
		return keys;
	}
};

}