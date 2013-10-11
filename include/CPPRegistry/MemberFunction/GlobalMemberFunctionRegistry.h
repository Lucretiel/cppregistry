#pragma once

#include <CPPRegistry/MemberFunction/MemberFunctionRegistry.h>

namespace registry
{

/*
 *  Named Global registry. Provides static methods to access a named global function
 *  registry. The NameTag template parameter allows multiple global instances of
 *  Registries with otherwise the same types to coexist.
 */
template<class NameTag, class Key, class Ret, class Type, class... Args>
class NamedGlobalMemberFunctionRegistry
{
private:
	typedef MemberFunctionRegistry<Key, Ret, Type, Args...> registry_type;

	static registry_type& get_registry()
	{
		static registry_type m_registry;
		return m_registry;
	}

public:
	typedef typename Key key_type;
	typedef typename Type object_type;
	typedef typename Ret return_type;
	typedef typename registry_type::function_type function_type;
	typedef typename registry_type::keys_type keys_type;

	//returns bool for static initializtion
	static bool register_function(const Key& key, function_type func)
	{
		get_registry().register_function(key, func);
		return true;
	}

	static function_type get_function(const Key& key)
	{
		return get_registry().get_function(key);
	}

	template<class T, class... DeterminedTypes>
	static Ret call_function(T&& object, const Key& key, DeterminedTypes&&... args)
	{
		return get_registry().call_function(
			std::forward<T>(object), key,
			std::forward<DeterminedTypes>(args)...);
	}

	static const keys_type& get_registered_functions()
	{
		return get_registry().get_registered_functions();
	}
};

}