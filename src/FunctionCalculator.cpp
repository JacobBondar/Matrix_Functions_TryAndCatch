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

FunctionCalculator::FunctionCalculator(std::istream& istr, std::ostream& ostr)
    : m_actions(createActions()), m_operations(createOperations()), 
      m_istr(istr), m_ostr(ostr), m_iss("") { }

void FunctionCalculator::run()
{
    getMaxOperation();

    do
    {
        m_iss.clear();
        m_iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        m_ostr << '\n';
        printOperations();
        m_ostr << "Enter command ('help' for the list of available commands): ";

        try
        {
            getUserCommand();

            const auto action = readAction();
            runAction(action);
        }
		catch (const std::runtime_error& e)
		{
			m_ostr << "Error: " << e.what() << '\n';
			continue;
		}
        
    } while (m_running);
}

void FunctionCalculator::eval()
{
    if (auto index = readOperationIndex(); index)
    {
        int size = 0;
        
        if (!(m_iss >> size) || size < 1 || size > MAX_MAT_SIZE)
		{
			throw std::out_of_range("Matrix size is out of the valid range (1-5).");
		}

        const auto& operation = m_operations[*index];
		int inputCount = operation->inputCount();
		auto matrixVec = std::vector<Operation::T>();
        if (inputCount > 1)
            m_ostr << "\nPlease enter " << inputCount << " matrices:\n";

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
}

void FunctionCalculator::del()
{
    if (auto i = readOperationIndex(); i)
    {
        m_operations.erase(m_operations.begin() + *i);
    }
}

void FunctionCalculator::help()
{
    m_ostr << "The available commands are:\n";
    for (const auto& action : m_actions)
    {
        m_ostr << "* " << action.command << action.description << '\n';
    }
    m_ostr << '\n';
}

void FunctionCalculator::exit()
{
    m_ostr << "Goodbye!\n";
    m_running = false;
}

void FunctionCalculator::getMaxOperation()
{
    while (true)
    {
        try
        {
            m_maxOperation = getNumber();
        }
		catch (const std::runtime_error& e)
		{
			std::cerr << "Error: " << e.what() << '\n';
			continue;
		}
        catch (const std::out_of_range& e)
        {
            std::cerr << "Error: " << e.what() << '\n';
            continue;
        }

        break;
    }
}

int FunctionCalculator::getNumber()
{
    int value;
    m_ostr << "\nEnter the maximum number of operations: ";

    if (!(m_istr >> value))
	{
        m_istr.clear();
        m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw std::runtime_error("Input is not a valid number.");
	}

	if (value < 2 || value > 100)
	{
		m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw std::out_of_range("Number is out of the valid range (2-100).");
	}

    m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return value;
}

void FunctionCalculator::printOperations() const
{
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

void FunctionCalculator::getUserCommand()
{
    std::string input;
    std::getline(m_istr, input);

	m_iss.clear();
	m_iss.str(input);
}

std::optional<int> FunctionCalculator::readOperationIndex()
{
    int i = 0;
    
	if (!(m_iss >> i))
	{
        m_iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw std::runtime_error("Invalid input. Please enter a valid operation index.");
	}

    if (i >= static_cast<int>(m_operations.size()) || i < 0)
    {
		throw std::out_of_range("Operation doesn't exist!");
        m_iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return {};
    }
    return i;
}

FunctionCalculator::Action FunctionCalculator::readAction()
{
    std::string action;
    if (!(m_iss >> action)) throw std::runtime_error("Command not found\n");

    const auto i = std::ranges::find(m_actions, action, &ActionDetails::command);

	if (i == m_actions.end())
	{
		std::cout << "wooooooooooooo\n";

		throw std::runtime_error("Command not found\n");
	}

    return i->action;
}

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
                //case Action::Iden:          unaryFunc<Identity>();      break;
                //case Action::Tran:          unaryFunc<Transpose>();      break;
			case Action::Resize:       resizeMaxOperations();      break;
            case Action::Scal:         unaryWithIntFunc<Scalar>(); break;
        }
    }
	catch (const std::out_of_range& e)
	{
        m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		m_ostr << "Error: " << e.what() << '\n';
	}
	catch (const std::runtime_error& e)
	{
        m_istr.clear();
        m_istr.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		m_ostr << "Error: " << e.what() << '\n';
	}
}

void FunctionCalculator::resizeMaxOperations()
{
	int value = getNumber();

    if (value < m_operations.size())
    {
		m_ostr << "You have inserted the number: " << value << 
            ". The number is under the the amount of the current operations.\n" << 
            "You have two choices:\n" <<
            "   - Enter 'cancel' to cancel the resizing.\n" <<
            "   - Enter 'del' to del functions to fit the new limit.\n";

        evalCommandResize(value);
    }

    else changeMaxOperation(value);
}

void FunctionCalculator::evalCommandResize(int value)
{
    do
    {
	    std::string command;
	    m_istr >> command;
	    if (command == "cancel") break;

	    else if (command == "del")
	    {
		    while (m_operations.size() > value)
		    {
				m_ostr << "\nDeleting operation #";

			    del();
		    }
            changeMaxOperation(value);
            break;
	    }
        m_ostr << "Invalid command! Try again: ";
    } while (true);
}

void FunctionCalculator::changeMaxOperation(int value)
{
	m_maxOperation = value;
	m_ostr << "The maximum number of operations is now: " << m_maxOperation << '\n';
}

FunctionCalculator::ActionMap FunctionCalculator::createActions() const
{
    return ActionMap
    {
        {
            "eval",
            "(uate) num n - compute the result of function #num on an n×n matrix "
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
            " num1 num2 - creates an operation that is the addition of the result of operation #num1 "
			"and the result of operation #num2",
            Action::Add
        },
         {
            "sub",
            " num1 num2 - creates an operation that is the subtraction of the result of operation #num1 "
			"and the result of operation #num2",
            Action::Sub
        },
        {
            "comp",
            "(osite) num1 num2 - creates an operation that is the composition of operation #num1 "
			"and operation #num2",
            Action::Comp
        },
        {
            "del",
            "(ete) num - delete operation #num from the operation list",
            Action::Del
		},
		{
			"resize",
			"(ize) num - resize the maximum number of operations to num (2 <= num <= 100)",
			Action::Resize
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

FunctionCalculator::OperationList FunctionCalculator::createOperations() const
{
    return OperationList
    {
        std::make_shared<Identity>(),
        std::make_shared<Transpose>(),
    };
}
