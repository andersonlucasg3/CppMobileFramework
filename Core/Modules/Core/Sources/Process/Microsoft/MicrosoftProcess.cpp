#include "MicrosoftProcess.h"

#include "Filesystem/Path.h"

void CWindowsProcess::SetCommandLine(int argc, char* argv[])
{
	ExecutablePath = argv[0];
	ExecutableContainerPath = GPath.RemoveLastPathComponent(ExecutablePath);

	for (int Index = 1; Index < argc; ++Index)
	{
		CommandLineArgs.Add(argv[Index]);
	}
}

const CString& CWindowsProcess::GetExecutablePath() const
{
	return ExecutablePath;
}

const CString& CWindowsProcess::GetContainerPath() const
{
	return ExecutableContainerPath;
}

const TArray<CString>& CWindowsProcess::GetCommandLineArgs() const
{
	return CommandLineArgs;
}