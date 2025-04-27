#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iosfwd>
#include <optional>
#include <iostream>
#include <stdexcept>
#include <sstream>

const int MAX_MAT_SIZE = 5;

class Operation;

class FunctionCalculator
{
public:
    FunctionCalculator(std::istream& istr, std::ostream& ostr);
    void run();

private:
    void eval();
    void del();
    void help();
    void exit();
    void getMaxOperation();
    int getNumber();
    void resizeMaxOperations();
    void evalCommandResize(int value);
    void changeMaxOperation(int value);

    template <typename FuncType>
    void binaryFunc()
    {
        if (auto f0 = readOperationIndex(), f1 = readOperationIndex(); f0 && f1)
        {
            if (m_operations.size() == m_maxOperation)
            {
                throw std::out_of_range("You have exceeded the limits of the number of the operations! Returning...");
            }

            m_operations.push_back(std::make_shared<FuncType>(m_operations[*f0], m_operations[*f1]));
        }
    }
    template <typename FuncType>
    void unaryFunc()
    {
    	m_operations.push_back(std::make_shared<FuncType>());
	}
    template <typename FuncType>
    void unaryWithIntFunc()
    {
        int i = 0;
        m_iss >> i;
        m_operations.push_back(std::make_shared<FuncType>(i));
    }
    void printOperations() const;
    void getUserCommand();

    enum class Action
    {
        Invalid,
        Eval,
        Iden,
        Tran,
        Scal,
        Sub,
        Add,
        Mul,
        Comp,
        Del,
        Resize,
        Help,
        Exit,
    };

    struct ActionDetails
    {
        std::string command;
        std::string description;
        Action action;
    };

    using ActionMap = std::vector<ActionDetails>;
    using OperationList = std::vector<std::shared_ptr<Operation>>;

    const ActionMap m_actions;
    OperationList m_operations;
    bool m_running = true;
    std::istream& m_istr;
    std::ostream& m_ostr;
    std::istringstream m_iss;
	int m_maxOperation;

    std::optional<int> readOperationIndex();
    Action readAction();

    void runAction(Action action);

    ActionMap createActions() const;
    OperationList createOperations() const ;
};
