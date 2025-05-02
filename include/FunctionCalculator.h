#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iosfwd>
#include <optional>
#include <iostream>
#include <stdexcept>
#include <sstream>

#include "Read.h"
#include "Utility.h"
#include "ReadException.h"
#include "FileException.h"
#include "OperationExceptionRange.h"
#include "OperationExceptionDigit.h"

const int MAX_MAT_SIZE = 5;

enum t_numArgs
{
    ZERO_ARGS,
    ONE_ARGS,
    TWO_ARGS
};

class Operation;

class FunctionCalculator
{
public:
    FunctionCalculator(std::istream& istr, std::ostream& ostr);
    void run();
    
    void executeCommand();
    void setStreams(std::string input);

private:
    struct ActionDetails
    {
        std::string command;
        std::string description;
        Action action;
    };

    using ActionMap = std::vector<ActionDetails>;
    using OperationList = std::vector<std::shared_ptr<Operation>>;

    void eval();
    void del();
    void help() const;
    void exit();
    void getMaxOperation();
    void resizeMaxOperations();
    void evalCommandResize(int value);
    void changeMaxOperation(int value);
    void validNumOfArguments(int wanted) const;
	void compare(bool valid) const;
    void read();
    void printNumMat(int inputCount) const;
    void printOperations() const;
    void getUserCommand();
    void runAction(Action action);
    void validDigit(int& value);
    int getNumber();
    int getSizeMat();
    int readOperationIndex();
    bool startDel(int value);
    Action readAction();
    ActionMap createActions() const;
    OperationList createOperations() const;

	template <typename ErrorType>
    void throwError(const std::string& message) const;
    template <typename ErrorType>
    void throwMatrix(std::string message) const;
    template <typename FuncType>
    void binaryFunc();
    template <typename FuncType>
    void unaryFunc();
    template <typename FuncType>
    void unaryWithIntFunc();

    OperationList m_operations;
    std::istream& m_istr;
    std::ostream& m_ostr;
    std::istringstream m_iss;
	Action m_currInput;
    const ActionMap m_actions;
    bool m_running = true;
	int m_maxOperation;
};

//-----------------------------------------------------------------------------

template <typename ErrorType>
void FunctionCalculator::throwError(const std::string& message) const
{
    if (m_currInput == Action::Read) throw ReadException(message + "\n");
    throw ErrorType(message);
}

//-----------------------------------------------------------------------------

template <typename ErrorType>
void FunctionCalculator::throwMatrix(std::string message) const
{
    m_istr.clear();
    m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    throwError<ErrorType>(message);
}

//-----------------------------------------------------------------------------

template <typename FuncType>
void FunctionCalculator::binaryFunc()
{
    validNumOfArguments(2);
    int f0 = readOperationIndex(), f1 = readOperationIndex();

    if (m_operations.size() >= m_maxOperation)
    {
        std::string message = "You have exceeded the limits of the number"
            " of the operations! Returning...\n";

        throwError<std::out_of_range>(message);
    }

    m_operations.push_back(std::make_shared<FuncType>(m_operations[f0],
                                                      m_operations[f1]));
}

//-----------------------------------------------------------------------------

template <typename FuncType>
void FunctionCalculator::unaryFunc()
{
    m_operations.push_back(std::make_shared<FuncType>());
}

//-----------------------------------------------------------------------------

template <typename FuncType>
void FunctionCalculator::unaryWithIntFunc()
{
    validNumOfArguments(ONE_ARGS);
    if (m_operations.size() >= m_maxOperation)
    {
        throwError<std::out_of_range>("You have exceeded the limits of the number"
            " of the operations! Returning...\n");
    }

    int i = 0;
    validDigit(i);

    if (i < MIN_ALLOWED_VALUE || i > MAX_ALLOWED_VALUE)
    {
        throwError<std::out_of_range>("Value is out of the allowed range!");
    }

    m_operations.push_back(std::make_shared<FuncType>(i));
}