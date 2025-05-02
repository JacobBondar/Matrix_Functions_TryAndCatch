#include "BinaryOperation.h"
#include <iostream>

//-----------------------------------------------------------------------------

BinaryOperation::BinaryOperation(const std::shared_ptr<Operation>& first, 
                                 const std::shared_ptr<Operation>& second)
    : m_first(first), m_second(second) {}

//-----------------------------------------------------------------------------

void BinaryOperation::print(std::ostream& ostr, bool first_print ) const
{
    if (!first_print)
        ostr << '(';
    first()->print(ostr);
    ostr << ' ';
    printSymbol(ostr);
    ostr << ' ';
    second()->print(ostr);
    if (!first_print)
        ostr << ')';
}
