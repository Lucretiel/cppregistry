#pragma once

#include <CPPRegistry\FunctionRegistry.h>

namespace registry
{

//TODO: support for constructor args.
//Note: Have tried to implement this. Trouble is the signature of TypeRegistry::make.
template<class Key, class Base>
class TypeRegistry
{
public:
	typedef Key key_type;
	typedef Base base_type;
	typedef Base* base_ptr;

private:
	typedef FunctionRegistry<key_type, base_ptr> registry_type;

public:
	typedef registry_type::keys_type keys_type;

private:
	registry_type m_registry;

	template<class Derived>
	static base_ptr make()
	{
		return Derived();
	}

public:
	template<class Derived>
	void register_type(const key_type& key)
	{
		m_registry.register_function(key, &make<Derived>);
	}

	base_ptr make_type(const key_type& key) const
	{
		auto maker = m_registry.get_function(key);
		if(maker)
			return maker();
		else
			return nullptr;
	}

	const keys_type& get_registered_types() const
	{
		return m_registry.get_registered_functions();
	}
};

}