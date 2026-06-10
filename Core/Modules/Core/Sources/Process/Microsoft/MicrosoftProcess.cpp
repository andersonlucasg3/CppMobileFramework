#include "MicrosoftProcess.h"

#include "Filesystem/Path.h"

void CMicrosoftProcess::SetCommandLine(int argc, char* argv[])
{
	ExecutablePath = argv[0];
	ExecutableContainerPath = GPath.RemoveLastPathComponent(ExecutablePath);

	for (int Index = 1; Index < argc; ++Index)
	{
		CommandLineArgs.Add(argv[Index]);
	}
}

const CString& CMicrosoftProcess::GetExecutablePath() const
{
	return ExecutablePath;
}

const CString& CMicrosoftProcess::GetContainerPath() const
{
	return ExecutableContainerPath;
}

const TArray<CString>& CMicrosoftProcess::GetCommandLineArgs() const
{
	return CommandLineArgs;
}