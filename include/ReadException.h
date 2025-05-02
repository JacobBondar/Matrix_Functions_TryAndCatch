#pragma once
#include <stdexcept>

class ReadException : public std::exception
{
public:
	ReadException(const std::string& message) : m_message(message) {};
	virtual const char* what() const noexcept override;

private:
	std::string m_message;
};