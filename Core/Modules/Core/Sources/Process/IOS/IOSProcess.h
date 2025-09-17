#pragma once

#include "Process/Mobile/MobileProcess.h"

#include "Templates/Array.h"

#include "String/String.h"

class CIOSProcess : public CMobileProcess
{
    CString ExecutablePath;
    CString ExecutableContainerPath;
    TArray<CString> CommandLineArgs;
    
public:
    CORE_API CIOSProcess() = default;
    CORE_API ~CIOSProcess() override = default;

    CORE_API void SetCommandLine(int argc, char* argv[]) override;
    CORE_API const CString& GetContainerPath() const override;
    CORE_API const TArray<CString>& GetCommandLineArgs() const override;

    CORE_API bool IsRunningInsideAppBundle() const;
};

typedef CIOSProcess CPlatformProcess;

extern const CIOSProcess& GAppleProcess;