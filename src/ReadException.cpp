#include "ReadException.h"

//-----------------------------------------------------------------------------

const char* ReadException::what() const noexcept
{
	return m_message.c_str();
}