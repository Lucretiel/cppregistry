#include <iostream>
#include "operator.h"

/*
 *  The operators.h file contains the declaration of a global function
 *  registry called Operators. The function registration takes place
 *  automatically, so functions can begin using functions registered to
 *  it right away, without any initialization boilerplate or other #includes.
 */
int main()
{
	using namespace std;
	auto& available_operators = GET_REGISTERED_FUNCTIONS(Operators);

	while(true)
	{
		cout << "Available operators:\n";
		for(const auto& op : available_operators)
			cout << "    " << op << '\n';
		cout << "Enter and operation and two numbers, or quit: ";
		std::string operation;
		int a = 0, b = 0;
		cin >> operation;
		if(operation == "quit")
			break;
		cin >> a >> b;
		auto function = GET_FUNCTION(Operators, operation);
		if(function)
			cout << "  Result: " << function(a, b) << '\n';
		else
			cout << "  " << operation << " is not a registered operation!\n";
	}

}





