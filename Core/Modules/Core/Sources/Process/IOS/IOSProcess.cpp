#include "IOSProcess.h"

#include "Filesystem/Path.h"

const CIOSProcess& GAppleProcess = static_cast<const CIOSProcess&>(GProcess);

void CIOSProcess::SetCommandLine(int argc, char* argv[])
{
    ExecutablePath = argv[0];
    ExecutableContainerPath = GPath.RemoveLastPathComponent(ExecutablePath);
    for (int Index = 1; Index < argc; ++Index)
    {
        CommandLineArgs.Add(argv[Index]);
    }
}

const CString& CIOSProcess::GetContainerPath() const
{
    return ExecutableContainerPath;
}

const TArray<CString>& CIOSProcess::GetCommandLineArgs() const
{
    return CommandLineArgs;
}

bool CIOSProcess::IsRunningInsideAppBundle() const
{
    return true;
}