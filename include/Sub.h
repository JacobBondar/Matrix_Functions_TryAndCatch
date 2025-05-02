#pragma once
#include "BinaryOperation.h"
#include <memory>


class Sub : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    T compute(const std::vector<T>& input) const override;
    void printSymbol(std::ostream& ostr) const override;
};
