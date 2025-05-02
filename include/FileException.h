#pragma once
#include <stdexcept>

class FileException : public std::exception
{
public:
	FileException(const std::string& message) : m_message(message) {};
	virtual const char* what() const noexcept override;

private:
	std::string m_message;
};