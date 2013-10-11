#pragma once

#include <unordered_map>
#include <unordered_set>
#include <type_traits>

namespace registry
{

//generic class for all registries. Actually stores things.
template<class Key, class Value>
class GenericRegistry
{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef std::unordered_set<key_type> keys_type;

private:
	typedef std::unordered_map<key_type, value_type> registry_type;

	registry_type m_registry;
	keys_type keys;

public:
	template<class T>
	void register_item(const Key& key, T&& value)
	{
		m_registry.emplace(key, std::forward<T>(value));
		keys.emplace(key);
	}

	const Value* get_item(const Key& key) const
	{
		auto result = m_registry.find(key);
		return result != m_registry.end() ?
			&result->second : nullptr;
	}

	Value* get_item(const Key& key)
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

//Slight specialization of GenericRegistry. Stores pointers. Returns nullptr on a failed lookup.
template<class Key, class Ptr>
class GenericPointerRegistry
{
private:
	typedef GenericRegistry<Key, Ptr> registry_type;
	registry_type m_registry;

public:
	typedef Key key_type;
	typedef typename std::remove_pointer<Ptr>::type value_type;
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

//Base class for storing function pointers of all kinds.
template<class Key, class Function>
class BaseFunctionRegistry
{
private:
	typedef GenericPointerRegistry<Key, Function> registry_type;
	registry_type m_registry;

public:
	typedef Function function_type;
	typedef Key key_type;
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

//Function registry. Stores, retrieves, and calls regular function pointers.
template<class Key, class Ret, class... Args>
class FunctionRegistry: public BaseFunctionRegistry<Key, Ret(*)(Args...)>
{
public:
	typedef Ret return_type;
	template<class... DeterminedArgs>
	Ret call_function(const Key& key, DeterminedArgs&&... args) const
	{
		auto func = get_function(key);
		if(func)
			return func(std::forward<DeterminedArgs>(args)...);
		else
			throw function_not_registered();
	}
};

//Member function registry. Stores, retrieves, and calls member functions.
template<class Key, class Ret, class Type, class...Args>
class MemberFunctionRegistry: public BaseFunctionRegistry<Key, Ret(Type::*)(Args...)>
{
public:
	typedef Type object_type;
	typedef Ret return_type;

	template<class T, class... DeterminedArgs>
	return_type call_function(
		T&& object,
		const key_type& key,
		DeterminedArgs&&... args) const
	{
		auto func = get_function(key);
		if(func)
			return MEMBER_FUNCTION(object, func)(std::forward<DeterminedArgs>(args)...);
		else
			throw function_not_registered();
	}
};

//Type registry. Stores and constructs instances of types of a given base class.
//TODO: support for constructor args.
//Note: Have tried to implement this. Trouble is the signature of TypeRegistry::make.
//Obviously it would assume that all Derived classes have the same signature.
//TODO: add support for allocators. Because EVERYONE uses allocators.
template<class Key, class Base>
class TypeRegistry
{
private:
	typedef FunctionRegistry<Key, Base*> registry_type;

public:
	typedef typename registry_type::keys_type keys_type;
	typedef Key key_type;
	typedef Base base_type;
	typedef Base* base_ptr;

private:
	registry_type m_registry;

	template<class Derived>
	static base_ptr make()
	{
		return new Derived();
	}

public:
	template<class Derived>
	void register_type(const Key& key)
	{
		m_registry.register_function(key, &make<Derived>);
	}

	Base* make_type(const Key& key) const
	{
		auto maker = m_registry.get_function(key);
		return maker ? maker() : nullptr;
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