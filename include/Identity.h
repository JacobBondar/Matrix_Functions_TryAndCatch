#pragma once
#include "UnaryOperation.h"

// Represents the identity operation
// Returns the same set that it gets as input
// Used as the leaf in every operation tree
class Identity : public UnaryOperation
{
public:
    using UnaryOperation::UnaryOperation;
	T compute(const std::vector<T>& input) const override;
    void print(std::ostream& ostr, bool first_print = false) const override;
};
