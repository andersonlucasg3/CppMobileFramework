#include "GoogleProcess.h"

#include "Templates/Array.h"

#include "Filesystem/Android/AndroidPath.h"

const CAndroidProcess& GAndroidProcess = static_cast<const CAndroidProcess&>(GProcess);

void CAndroidProcess::SetCommandLine(int argc, char* argv[])
{
    // TODO: implement commandline support
}

const TArray<CString>& CAndroidProcess::GetCommandLineArgs() const
{
    static TArray<CString> Temp;

    return Temp;
}

const CString& CAndroidProcess::GetContainerPath() const
{
    // Android's container is the files path
    return GAndroidPath.GetFilesPath();
}