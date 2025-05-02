#include "Comp.h"
#include <iostream>

//-----------------------------------------------------------------------------

int Comp::inputCount() const
{
    return first()->inputCount() + second()->inputCount() - 1;
}

//-----------------------------------------------------------------------------

Operation::T Comp::compute(const std::vector<T>& input) const
{
    const auto resultOfFirst = first()->compute(input);
    auto firstCount = first()->inputCount();
    std::vector input2(input.begin() + firstCount, input.end());
	input2.insert(input2.begin(), resultOfFirst);
    return second()->compute(input2);
}

//-----------------------------------------------------------------------------

void Comp::printSymbol(std::ostream& ostr) const
{
    ostr << " -> ";
}
