#pragma once
#include <stdexcept>

class OperationExceptionRange : public std::exception
{
public:
	OperationExceptionRange(const std::string& message) : m_message(message) {};
	virtual const char* what() const noexcept override;

private:
	std::string m_message;
};
