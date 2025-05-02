#include "Read.h"
#include "FunctionCalculator.h"

//-----------------------------------------------------------------------------

Read::Read(FunctionCalculator* funcPtr, std::string pathName) 
	: m_funcPtr(funcPtr), m_inputFile(pathName)
{
	if (!m_inputFile.is_open())
	{
		throw FileException("Failed to open the file.");
	}
}

//-----------------------------------------------------------------------------

Read::~Read()
{
	if (m_inputFile.is_open())
	{
		m_inputFile.close();
	}
}

//-----------------------------------------------------------------------------

void Read::readFile()
{
	std::string lineCommand;
	while (std::getline(m_inputFile, lineCommand))
	{
		try 
		{
			m_funcPtr->setStreams(lineCommand);
			m_funcPtr->executeCommand();
		} 
		catch (const ReadException& e)
		{
			printException(e.what(), lineCommand);
			std::cin.clear();

			std::string input;
			while(std::cin >> input)
			{
				std::cout << "\n================================================" <<
							 "==============================\n";
				if (input == "Yes") break;
				else if (input == "No") return;
				std::cout << "\nInvalid input! Please enter 'Yes' or 'No': ";
			}
		}
	}
}

//-----------------------------------------------------------------------------

void Read::printException(std::string message, std::string lineCommand)
{
	std::cout << "\nThere was a problem in the file with the command: "
		<< lineCommand << '\n';
	std::cerr << message << '\n';
	std::cout << "Do you want to continue?" <<
				 "\nEnter 'Yes' to continue\nEnter 'No' to stop: ";
}