#include "MicrosoftEnvironment.h"

#if PLATFORM_WINDOWS

const char* CWindowsEnvironment::NewLine() const
{
	return "\r\n";
}

#endif