#pragma once

#include <CPPRegistry/Base/BaseFunctionRegistry.h>

namespace registry 
{

template<class Key, class Ret, class... Args>
class FunctionRegistry: public detail::BaseFunctionRegistry<Key, Ret, Ret()(Args...)>
{
public:
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

}