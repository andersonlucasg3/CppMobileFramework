#include "MicrosoftEnvironment.h"

#if PLATFORM_WINDOWS

const char* CMicrosoftEnvironment::NewLine() const
{
	return "\r\n";
}

#endif