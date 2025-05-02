#include "FunctionCalculator.h"
#include "SquareMatrix.h"
#include "Add.h"
#include "Sub.h"
#include "Comp.h"
#include "Identity.h"
#include "Transpose.h"
#include "Scalar.h"

#include <iostream>
#include <algorithm>

//-----------------------------------------------------------------------------

FunctionCalculator::FunctionCalculator(std::istream& istr, std::ostream& ostr)
    : m_actions(createActions()), m_operations(createOperations()), 
      m_istr(istr), m_ostr(ostr), m_iss("") { }

//-----------------------------------------------------------------------------

void FunctionCalculator::run()
{
    getMaxOperation();

    do
    {
        m_ostr << '\n';
        printOperations();
        m_ostr << "Enter command ('help' for the list of available commands): ";

        try
        {
			m_currInput = Action::Invalid;
            getUserCommand();
            executeCommand();
        }
		catch (const std::exception& e)
		{
			m_ostr << "Error: " << e.what() << '\n';
			continue;
		}
        
    } while (m_running);
}

//-----------------------------------------------------------------------------

void FunctionCalculator::executeCommand()
{
    const auto action = readAction();
    runAction(action);
}

//-----------------------------------------------------------------------------

void FunctionCalculator::setStreams(std::string input)
{
	m_iss.clear();
    m_iss.str(input);
}

//-----------------------------------------------------------------------------

void FunctionCalculator::eval()
{
    try
    {
        validNumOfArguments(TWO_ARGS);
        int index = readOperationIndex();
        int size = getSizeMat();

        const auto& operation = m_operations[index];
        int inputCount = operation->inputCount();
        auto matrixVec = std::vector<Operation::T>();
        printNumMat(inputCount);

        for (int i = 0; i < inputCount; ++i)
        {
            auto input = Operation::T(size);
            m_ostr << "\nEnter a " << size << "x" << size << " matrix:\n";
            m_istr >> input;

            matrixVec.push_back(input);
        }

        m_ostr << "\n";
        operation->print(m_ostr, matrixVec);

        m_ostr << " = \n" << operation->compute(matrixVec);
    }
    // Catches for the matrices alone
    catch (const std::runtime_error& e)
    {
        throwMatrix<std::runtime_error>(e.what());
    }
    catch (const std::out_of_range& e)
    {
        throwMatrix<std::out_of_range>(e.what());
    }
}

//-----------------------------------------------------------------------------

void FunctionCalculator::printNumMat(int inputCount) const
{
    if (inputCount > 1)
        m_ostr << "\nPlease enter " << inputCount << " matrices:\n";
}

//-----------------------------------------------------------------------------

int FunctionCalculator::getSizeMat()
{
    int size;
    validDigit(size);

    if (size < 1 || size > MAX_MAT_SIZE)
    {
        throw OperationExceptionRange
            ("Invalid size. Please size in the range of (1 - 5).");
    }
    return size;
}

//-----------------------------------------------------------------------------

void FunctionCalculator::validNumOfArguments(int wanted) const
{
    std::string arguments;
	std::istringstream tempIss(m_iss.str());

	int counterArguments = -1;
    while (tempIss >> arguments)
    {
        ++counterArguments;
        compare(counterArguments > wanted);
    }
    compare(counterArguments < wanted);
}

//-----------------------------------------------------------------------------

void FunctionCalculator::compare(bool valid) const
{
    if (valid)
    {
        throw OperationExceptionRange
            ("Invalid number of arguments. Please enter a valid number of arguments.");
    }
}

//-----------------------------------------------------------------------------

void FunctionCalculator::del()
{
    validNumOfArguments(ONE_ARGS);
    int i = readOperationIndex();
    m_operations.erase(m_operations.begin() + i);
}

//-----------------------------------------------------------------------------

void FunctionCalculator::help() const
{
    validNumOfArguments(ZERO_ARGS);
    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
    {
        m_ostr << "* " << action.command << action.description << '\n';
    }
    m_ostr << '\n';
}

//-----------------------------------------------------------------------------

void FunctionCalculator::exit()
{
    validNumOfArguments(ZERO_ARGS);
    m_ostr << "Goodbye!\n";
    m_running = false;
}

//-----------------------------------------------------------------------------

void FunctionCalculator::getMaxOperation()
{
	std::string maxValue;
    while (true)
    {
        try
        {
            m_ostr << "Enter the maximum number of operations: ";
			m_istr >> maxValue;
            m_iss.str(maxValue);

            m_maxOperation = getNumber();
            break;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << '\n';
        }
		m_iss.clear();
    }
}

//-----------------------------------------------------------------------------

int FunctionCalculator::getNumber()
{
    int value;
    validDigit(value);

	if (value < 2 || value > 100)
	{
		throwError<std::out_of_range>
			("Number is out of the valid range (2-100).");
	}
	return value;
}

//-----------------------------------------------------------------------------

void FunctionCalculator::validDigit(int& value)
{
    std::string input;
    std::size_t pos;

    m_iss >> input;
    value = std::stoi(input, &pos);

    if (pos != input.size())
    {
        throw OperationExceptionRange("Input is not a valid number.");
    }
}

//-----------------------------------------------------------------------------

void FunctionCalculator::printOperations() const
{
    m_ostr << "============================================================" <<
              "==================\n\n";
    m_ostr << "List of available matrix operations with the limit of - '" << 
       m_maxOperation << "' matrixes as input:\n";
    for (decltype(m_operations.size()) i = 0; i < m_operations.size(); ++i)
    {
        m_ostr << i << ". ";
        m_operations[i]->print(m_ostr,true);
        m_ostr << '\n';
    }
    m_ostr << '\n';
}

//-----------------------------------------------------------------------------

void FunctionCalculator::getUserCommand()
{
    std::string input;
    while (true)
    {
	    std::getline(m_istr, input);
        if (!input.empty()) break;
    }

	m_iss.clear();
	m_iss.str(input);
}

//-----------------------------------------------------------------------------

int FunctionCalculator::readOperationIndex()
{
    int i = 0;
    validDigit(i);

	if (i < 0 ||
        i >= static_cast<int>(m_operations.size()))
	{
        throw OperationExceptionDigit("Invalid input. Please enter a valid operation index.");
	}
    return i;
}

//-----------------------------------------------------------------------------

Action FunctionCalculator::readAction()
{
    std::string action;
    if (!(m_iss >> action))
    {
		throwError<std::runtime_error>("Command not found\n");
    }

    const auto i = std::ranges::find(m_actions, action, &ActionDetails::command);

	if (i == m_actions.end())
	{
        throwError<std::runtime_error>("Command not found\n");
	}
    
    return i->action;
}

//-----------------------------------------------------------------------------

void FunctionCalculator::runAction(Action action)
{
    try
    {
        switch (action)
        {
            default:
                m_ostr << "Unknown enum entry used!\n";
                break;

            case Action::Eval:         eval();                     break;
            case Action::Add:          binaryFunc<Add>();          break;
            case Action::Sub:          binaryFunc<Sub>();          break;
            case Action::Comp:         binaryFunc<Comp>();         break;
            case Action::Del:          del();                      break;
            case Action::Help:         help();                     break;
            case Action::Exit:         exit();                     break;
			case Action::Resize:       resizeMaxOperations();      break;
            case Action::Scal:         unaryWithIntFunc<Scalar>(); break;
            case Action::Read:         read();                     break;
        }
    }
	catch (const FileException& e)
    {
        throwError<std::out_of_range>(e.what());
    }
    catch (const OperationExceptionDigit& e)
    {
        throwError<std::out_of_range>(e.what());
    }
    catch (const OperationExceptionRange& e)
    {
        throwError<std::out_of_range>(e.what());
    }
    catch (const std::invalid_argument& e)
    {
        throwError<std::out_of_range>(e.what());
    }
}

//-----------------------------------------------------------------------------

void FunctionCalculator::read()
{
    std::string pathName;
    m_iss.get(); // avoid the spacebar
    std::getline(m_iss, pathName);
    m_currInput = Action::Read; // update we are reading from a file

    auto r = Read(this, pathName);
    r.readFile();
}

//-----------------------------------------------------------------------------

void FunctionCalculator::resizeMaxOperations()
{
    validNumOfArguments(ONE_ARGS);
	int value = getNumber();

    if (value < m_operations.size())
    {
		m_ostr << "\nYou are trying to resize to the number : " << value << 
            ". The number is under the the amount of the current operations.\n" << 
            "You have two choices:\n" <<
            "   - Enter 'cancel' to cancel the resizing.\n" <<
            "   - Enter 'del' to del functions to fit the new limit.\n";

        evalCommandResize(value);
    }

    else changeMaxOperation(value);
}

//-----------------------------------------------------------------------------

void FunctionCalculator::evalCommandResize(int value)
{
    do
    {
	    std::string command;
	    m_istr >> command;
        m_ostr << '\n';
	    if (command == "cancel") break;

	    else if (command == "del")
	    {
		    while (true)
		    {
                if (startDel(value)) break;
		    }
            changeMaxOperation(value);
            break;
	    }
        m_ostr << "Invalid command! Try again: ";
    } while (true);
}

//-----------------------------------------------------------------------------

bool FunctionCalculator::startDel(int value)
{
    printOperations();
    m_ostr << "You need to erase " << m_operations.size() - value <<
        " more operations:";
    m_ostr << "\nDelete operation #";

    m_iss.clear();
    std::string index;
    m_istr >> index;
    m_ostr << '\n';
    m_iss.str("del " + index);
    m_iss >> index; // To delete "del" for the .str()

    del();

    if (m_operations.size() <= value) return true;
    return false;
}

//-----------------------------------------------------------------------------

void FunctionCalculator::changeMaxOperation(int value)
{
	m_maxOperation = value;
	m_ostr << "The maximum number of operations is now: " 
           << m_maxOperation << '\n';
}

//-----------------------------------------------------------------------------

FunctionCalculator::ActionMap FunctionCalculator::createActions() const
{
    return ActionMap
    {
        {
            "eval",
            "(uate) num n - compute the result of function #num on an nxn matrix "
			"(that will be prompted)",
            Action::Eval
        },
        {
            "scal",
            "(ar) val - creates an operation that multiplies the "
			"given matrix by scalar val",
            Action::Scal
        },
        {
            "add",
            " num1 num2 - creates an operation that is the addition of the result "
			"of operation #num1 and the result of operation #num2",
            Action::Add
        },
         {
            "sub",
            " num1 num2 - creates an operation that is the subtraction of the result "
			"of operation #num1 and the result of operation #num2",
            Action::Sub
        },
        {
            "comp",
            "(osite) num1 num2 - creates an operation that is the composition of "
			"operation #num1 and operation #num2",
            Action::Comp
        },
        {
            "del",
            "(ete) num - delete operation #num from the operation list",
            Action::Del
		},
		{
			"resize",
			" num - resize the maximum number of operations to num (2 <= num <= 100)",
			Action::Resize
		},
        {
            "read",
            " pathFile - execute operations from a file",
            Action::Read
        },
        {
            "help",
            " - print this command list",
            Action::Help
        },
        {
            "exit",
            " - exit the program",
            Action::Exit
        }
    };
}

//-----------------------------------------------------------------------------

FunctionCalculator::OperationList FunctionCalculator::createOperations() const
{
    return OperationList
    {
        std::make_shared<Identity>(),
        std::make_shared<Transpose>(),
    };
}