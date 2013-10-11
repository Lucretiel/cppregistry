#pragma once

#include <CPPRegistry/Base/GenericPointerRegistry.h>

/*
 *  Convenience class to reduce code duplication between FunctionRegistry
 *  and MemberFunctionRegistry
 */

namespace registry
{

namespace detail
{

template<class Key, class Ret, class Function>
class BaseFunctionRegistry
{
private:
	typedef GenericPointerRegistry<Key, Function> registry_type;
	registry_type m_registry;

public:
	//No instansiations for you.
	virtual ~BaseFunctionRegistry() =0 {}

	typedef Function function_type;
	typedef Key key_type;
	typedef Ret return_type;
	typedef typename registry_type::keys_type keys_type;

	void register_function(const Key& key, function_type function)
	{
		m_registry.register_pointer(key, function);
	}

	const function_type get_function(const Key& key) const
	{
		return m_registry.get_pointer(key);
	}

	function_type get_function(const Key& key)
	{
		return m_registry.get_pointer(key);
	}

	const keys_type& get_registered_functions()
	{
		return m_registry.get_registered_pointers();
	}
};

}

/*
 *  Todo: determine whether to store the bad key in the exception. I'd rather
 *  not have to add a dependency on std::string.
 */
class function_not_registered
{};

}