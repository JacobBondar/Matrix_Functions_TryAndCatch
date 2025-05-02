#include "Add.h"
#include <iostream>

//-----------------------------------------------------------------------------

Operation::T Add::compute(const std::vector<T>& input) const
{
    const auto a = first()->compute(input);
    auto firstCount = first()->inputCount();
	//remove the firstCount elements from the input vector, and put in a new vector
	std::vector input2(input.begin() + firstCount, input.end());
    const auto b = second()->compute(input2);

    return a + b;
}

//-----------------------------------------------------------------------------

void Add::printSymbol(std::ostream& ostr) const
{
    ostr << '+';
}