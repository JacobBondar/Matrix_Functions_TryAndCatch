#pragma once
#include <stdexcept>

//-----------------------------------------------------------------------------

class OperationExceptionDigit : public std::exception
{
public:
	OperationExceptionDigit(const std::string& message) : m_message(message) {};
	virtual const char* what() const noexcept override;

private:
	std::string m_message;
};
