#pragma once

#include "Operation.h"

#include <memory>


class BinaryOperation : public Operation
{
public:
    BinaryOperation(const std::shared_ptr<Operation>& arg1, const std::shared_ptr<Operation>& arg2);
	int inputCount() const override { return m_first->inputCount() + m_second->inputCount(); }
protected:
    const std::shared_ptr<Operation>& first() const { return m_first; }
    const std::shared_ptr<Operation>& second() const { return m_second; }
    virtual void printSymbol(std::ostream& ostr) const = 0;
    void print(std::ostream& ostr, bool first_print =false) const override;

private:
    const std::shared_ptr<Operation> m_first;
    const std::shared_ptr<Operation> m_second;
};
