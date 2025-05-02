#include "OperationExceptionRange.h"

//-----------------------------------------------------------------------------

const char* OperationExceptionRange::what() const noexcept
{
	return m_message.c_str();
}