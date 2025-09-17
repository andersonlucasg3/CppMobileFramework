#include "MacProcess.h"

#include "Filesystem/Path.h"

const CMacProcess& GAppleProcess = static_cast<const CMacProcess&>(GProcess);

void CMacProcess::SetCommandLine(int argc, char* argv[])
{
    ExecutablePath = argv[0];
    ExecutableContainerPath = GPath.RemoveLastPathComponent(ExecutablePath);
    for (int Index = 1; Index < argc; ++Index)
    {
        CommandLineArgs.Add(argv[Index]);
    }
}

const CString& CMacProcess::GetExecutablePath() const
{
    return ExecutablePath;
}

const CString& CMacProcess::GetContainerPath() const
{
    return ExecutableContainerPath;
}

const TArray<CString>& CMacProcess::GetCommandLineArgs() const
{
    return CommandLineArgs;
}

bool CMacProcess::IsRunningInsideAppBundle() const
{
    return ExecutableContainerPath.EndsWith(".app");
}