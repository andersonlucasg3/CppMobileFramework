#pragma once

#include "Process/Mobile/MobileProcess.h"

class CAndroidProcess : public CMobileProcess
{
public:
    CORE_API ~CAndroidProcess() override = default;

    CORE_API void SetCommandLine(int argc, char* argv[]) override;
    CORE_API const TArray<CString>& GetCommandLineArgs() const override;

    CORE_API const CString& GetContainerPath() const override;
};

typedef CAndroidProcess CPlatformProcess;

extern const CAndroidProcess& GAndroidProcess;