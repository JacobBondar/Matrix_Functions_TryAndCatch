#include "Transpose.h"


Operation::T Transpose::compute(const std::vector<T>& input) const
{
    return input.front().Transpose();
}


void Transpose::print(std::ostream& ostr, bool first_print) const
{
    (void)first_print; // Cast to void to avoid unused parameter warning
    ostr << "tran";
}
