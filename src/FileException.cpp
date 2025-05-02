#include "FileException.h"

//-----------------------------------------------------------------------------

const char* FileException::what() const noexcept
{
	return m_message.c_str();
}