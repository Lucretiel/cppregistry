#pragma once

#include <CPPRegistry\detail\BaseFunctionRegistry.h>

//Helper to call member function pointers.
#define MEMBER_FUNCTION(OBJECT, FUNC) \
	((OBJECT).(*FUNC))

namespace registry
{

template<class Key, class Ret, class Type, class...Args>
class MemberFunctionRegistry: public detail::BaseFunctionRegistry<Key, Ret(Type::*)(Args...)>
{
public:
	typedef Ret return_type;
	typedef Key key_type;
	typedef Type object_type;

	template<class T, class... DeterminedArgs>
	return_type call_member_function(T&& object, const key_type& key, DeterminedArgs&&... args)
	{
		auto func = get_function(key);
		if(func)
			return MEMBER_FUNCTION(object, func)(std::forward<DeterminedArgs>(args)...);
		else
			throw function_not_registered();
	}
};

}