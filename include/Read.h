#pragma once
#include <fstream>
#include <stdexcept>
#include "utility.h"

class FunctionCalculator;

class Read
{
public:
	Read(FunctionCalculator* funcPtr, std::string pathName);
	~Read();
	void readFile();

private:
	FunctionCalculator* m_funcPtr;
	std::ifstream m_inputFile;

	void printException(std::string message, std::string lineCommand);
};