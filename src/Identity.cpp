#include "Identity.h"

#include <iostream>


Operation::T Identity::compute(const std::vector<T>& input) const
{
    return input.front();
}


void Identity::print(std::ostream& ostr, bool first_print) const
{
    (void)first_print; // Cast to void to avoid unused parameter warning
    ostr << "id";
}
