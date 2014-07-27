#pragma once

#include <unordered_map>
#include <unordered_set>
#include <type_traits>

namespace registry
{

//Class for storing function pointers. Returns nullptr on failed lookups.
template<class Key, class FunctionPtr>
class BaseFunctionRegistry
{
public:
	typedef Key key_type;
	typedef FunctionPtr func_ptr_type;
	typedef typename std::remove_pointer<FunctionPtr>::type func_type;
	typedef std::unordered_set<Key> keys_type;

private:
	typedef std::unordered_map<Key, FunctionPtr> registry_type;

	registry_type m_registry;
	keys_type keys;

public:
	void register_function(Key key, FunctionPtr value)
	{
		m_registry.emplace(key, value);
		keys.emplace(std::move(key));
	}

	FunctionPtr get_function(const Key& key) const
	{
		auto result = m_registry.find(key);
		return result != m_registry.end() ?
			result->second : nullptr;
	}

	const keys_type& get_registered_functions() const
	{
		return keys;
	}
};

//Function registry. Stores, retrieves, and calls regular function pointers.
template<class Key, class Ret, class... Args>
class FunctionRegistry: public BaseFunctionRegistry<Key, Ret(*)(Args...)>
{
public:
	typedef Ret return_type;

	Ret call_function(const Key& key, Args&&... args) const
	{
		auto func = get_function(key);
		if(func)
			return func(std::forward<Args>(args)...);
		else
			throw function_not_registered();
	}
};

#define MEMBER_FUNCTION(OBJECT, FUNC) ((OBJECT).*(FUNC))
//Member function registry. Stores, retrieves, and calls member functions.
template<class Key, class Ret, class Type, class... Args>
class MemberFunctionRegistry: public BaseFunctionRegistry<Key, Ret(Type::*)(Args...)>
{
public:
	typedef Type object_type;
	typedef Ret return_type;

	//Call with lvalue
	Ret call_function(Type& object, const Key& key, Args&&... args) const
	{
		auto func = get_function(key);
		if(func)
			return MEMBER_FUNCTION(object, func)(std::forward<Args>(args)...);
		else
			throw function_not_registered();
	}

	//Call with rvalue
	Ret call_function(Type&& object, const Key& key) const
	{
		return call_function(object);
	}
};
#undef MEMBER_FUNCTION

//Type registry. Stores and constructs instances of types of a given base class.
template<class Key, class Base, class... Args>
class TypeRegistry
{
private:
	typedef FunctionRegistry<Key, Base*, Args&&...> registry_type;

public:
	typedef typename registry_type::keys_type keys_type;
	typedef Key key_type;
	typedef Base base_type;
	typedef Base* base_ptr;

private:
	registry_type m_registry;

	template<class Derived>
	static Base* make(Args&&... args)
	{
		return new Derived(std::forward<Args>(args)...);
	}

public:
	template<class Derived>
	void register_type(Key key)
	{
		static_assert(std::is_base_of<Base, Derived>::value,
			"Must register a type derrived from the Base");
		m_registry.register_function(std::move(key), &make<Derived>);
	}

	Base* make_type(const Key& key, Args&&... args) const
	{
		auto maker = m_registry.get_function(key);
		return maker ? maker(std::forward<Args>(args)...) : nullptr;
	}

	const keys_type& get_registered_types() const
	{
		return m_registry.get_registered_functions();
	}
};

/*
 *  Global registry. Provides static methods to access a named global function
 *  registry. The NameTag template parameter allows multiple global instances of
 *  Registries with otherwise the same types to coexist. The macros manipulate
 *  this class.
 */
template<class NameTag, class Key, class Ret, class... Args>
class GlobalFunctionRegistry
{
private:
	typedef FunctionRegistry<Key, Ret, Args...> registry_type;

	static registry_type& get_registry()
	{
		static registry_type m_registry;
		return m_registry;
	}

public:
	typedef Key key_type;
	typedef Ret return_type;
	typedef typename registry_type::func_type func_type;
	typedef typename registry_type::func_ptr_type func_ptr_type;
	typedef typename registry_type::keys_type keys_type;

	//returns bool for static initializtion
	static bool register_function(Key key, func_ptr_type func)
	{
		get_registry().register_function(std::move(key), func);
		return false;
	}

	static func_ptr_type get_function(const Key& key)
	{
		return get_registry().get_function(key);
	}

	template<class... DeterminedTypes>
	static Ret call_function(const Key& key, DeterminedTypes&&... args)
	{
		return get_registry().call_function(key, std::forward<DeterminedTypes>(args)...);
	}

	static const keys_type& get_registered_functions()
	{
		return get_registry().get_registered_functions();
	}
};

template<class NameTag, class Key, class Ret, class Type, class... Args>
class GlobalMemberFunctionRegistry
{
private:
	typedef MemberFunctionRegistry<Key, Ret, Type, Args...> registry_type;

	static registry_type& get_registry()
	{
		static registry_type m_registry;
		return m_registry;
	}

public:
	typedef Key key_type;
	typedef Type object_type;
	typedef Ret return_type;
	typedef typename registry_type::func_type func_type;
	typedef typename registry_type::func_ptr_type func_ptr_type;
	typedef typename registry_type::keys_type keys_type;

	//returns bool for static initializtion
	static bool register_function(Key key, func_ptr_type func)
	{
		get_registry().register_function(std::move(key), func);
		return true;
	}

	static func_ptr_type get_function(const Key& key)
	{
		return get_registry().get_function(key);
	}

	static Ret call_function(Type& object, const Key& key, Args&&... args)
	{
		return get_registry().call_function(
			object, key, std::forward<Args>(args)...);
	}

	static Ret call_function(Type&& object, const Key& key, Args&&... args)
	{
		return call_function(object, key, std::forward<Args>(args)...);
	}

	static const keys_type& get_registered_functions()
	{
		return get_registry().get_registered_functions();
	}
};

template<class NameTag, class Key, class Base>
class GlobalTypeRegistry
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
	static bool register_type(Key key)
	{
		get_registry().template register_type<Derived>(std::move(key));
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
