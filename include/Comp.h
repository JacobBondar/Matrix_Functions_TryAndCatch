#pragma once
#include "BinaryOperation.h"
#include <string>
#include <memory>


class Comp : public BinaryOperation
{
public:
    using BinaryOperation::BinaryOperation;
    int inputCount() const override;
    T compute(const std::vector<T>& input) const override;
    void printSymbol(std::ostream& ostr) const override;
};
