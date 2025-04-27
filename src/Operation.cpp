#include "Operation.h"

#include <iostream>


void Operation::print(std::ostream& ostr, const std::vector<T>& input) const
{
	print(ostr);
	for (int i = 0; i < inputCount(); ++i)
	{
		ostr << "(\n" << input[i] << ")";
	}
}
