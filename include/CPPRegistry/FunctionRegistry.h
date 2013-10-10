#pragma once

#include <CPPRegistry\detail\BaseFunctionRegistry.h>

namespace registry 
{

template<class Key, class Ret, class... Args>
class FunctionRegistry: public detail::BaseFunctionRegistry<Key, Ret(*)(Args...)>
{
public:
	typedef Ret return_type;
	typedef Key key_type;
	
	template<class... DeterminedArgs>
	return_type call_function(const key_type& key, DeterminedArgs&&... args) const
	{
		auto func = get_function(key);
		if(func)
			return func(std::forward<DeterminedArgs>(args)...);
		else
			throw function_not_registered();
	}
};

}