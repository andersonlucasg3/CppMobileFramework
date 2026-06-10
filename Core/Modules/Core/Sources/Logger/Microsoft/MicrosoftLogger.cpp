#include "MicrosoftLogger.h"

#if PLATFORM_WINDOWS

#include "Process/Process.h"
#include "Environment/Environment.h"

#include <Windows.h>
#include <iostream>

void CMicrosoftLogger::WriteLogLine(const std::string& LogLine) const
{
	std::string Line = LogLine + GEnvironment.NewLine();

	Super::WriteLogLine(Line);

	std::cout << Line << GEnvironment.NewLine();

	OutputDebugStringA(Line.c_str());
}

CMicrosoftLogger::CMicrosoftLogger() : Super()
{
	if (AttachConsole(ATTACH_PARENT_PROCESS)) 
	{
		FILE* f_out;
		freopen_s(&f_out, "CONOUT$", "w", stdout);  // Redirect stdout to console
		freopen_s(&f_out, "CONOUT$", "w", stderr);  // Redirect stderr to console
	}
}

#endif