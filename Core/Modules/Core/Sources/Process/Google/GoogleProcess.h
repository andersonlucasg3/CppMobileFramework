#pragma once

#include "Process/Mobile/MobileProcess.h"

class CGoogleProcess : public CMobileProcess
{
public:
    CORE_API ~CGoogleProcess() override = default;

    CORE_API void SetCommandLine(int argc, char* argv[]) override;
    CORE_API const TArray<CString>& GetCommandLineArgs() const override;

    CORE_API const CString& GetContainerPath() const override;
};

typedef CGoogleProcess CPlatformProcess;

extern const CGoogleProcess& GAndroidProcess;