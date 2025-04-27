#pragma once

#include "SquareMatrix.h"

#include <vector>
#include <iosfwd>


// Represents an operation on sets
class Operation
{
public:
    using T = SquareMatrix<int>;
    virtual ~Operation() = default;

    // Return the number of inputs (the range size) expected by compute()
    virtual int inputCount() const = 0;

    // Computes the resulted set
    virtual T compute(const std::vector<T>& input) const =0;

    // Prints the operation with generic name for the sets or with the actual input arguments
    virtual void print(std::ostream& ostr, bool first_print = false) const = 0;

    virtual void print(std::ostream& ostr, const std::vector<T>& input) const;
};
