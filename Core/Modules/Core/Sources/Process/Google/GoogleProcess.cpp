#include "GoogleProcess.h"

#include "Templates/Array.h"

#include "Filesystem/Android/AndroidPath.h"

const CGoogleProcess& GAndroidProcess = static_cast<const CGoogleProcess&>(GProcess);

void CGoogleProcess::SetCommandLine(int argc, char* argv[])
{
    // TODO: implement commandline support
}

const TArray<CString>& CGoogleProcess::GetCommandLineArgs() const
{
    static TArray<CString> Temp;

    return Temp;
}

const CString& CGoogleProcess::GetContainerPath() const
{
    // Android's container is the files path
    return GAndroidPath.GetFilesPath();
}