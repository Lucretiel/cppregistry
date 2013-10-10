#pragma once

#include <exception>
#include <type_traits>
#include <CPPRegistry\GenericRegistry.h>

namespace registry
{

namespace detail
{

template<class Key, class FunctionPtrType>
class BaseFunctionRegistry
{
public:
	typedef Key key_type;
	typedef FunctionPtrType function_type;
	typedef std::result_of<FunctionType>::type return_type;

private:
	typedef GenericRegistry<key_type, function_ptr_type> registry_type;
	registry_type m_registry;

public:
	typedef typename registry_type::keys_type keys_type;

	void register_function(const key_type& key, function_type func)
	{
		m_registry.register_item(key, func);
	}

	function_ptr_type get_function(const Key& key) const
	{
		auto result = m_registry.get_item(key);
		return function_type ? *function_type : nullptr;
	}

	const keys_type& get_registered_functions() const
	{
		return m_registry.get_registered_items();
	}
};

}

class function_not_registered : public std::exception
{
public:
	const char* what() const override
	{
		return "Tried to call a function with an unregistered key";
	}
};

}