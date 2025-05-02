#include "OperationExceptionDigit.h"

//-----------------------------------------------------------------------------

const char* OperationExceptionDigit::what() const noexcept
{
	return m_message.c_str();
}