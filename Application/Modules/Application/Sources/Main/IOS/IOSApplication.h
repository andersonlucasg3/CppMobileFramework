#pragma once

#include "Main/Application.h"
#include "Object/ClassMacros.h"

class CIOSApplication : public CApplication
{
public:
    APPLICATION_API CIOSApplication() = default;
    APPLICATION_API ~CIOSApplication() override = default;

    APPLICATION_API int Run(int argc, char* argv[]) override;

    APPLICATION_API static CIOSApplication* Shared();
private:
    static CIOSApplication* _shared;
};

typedef CIOSApplication CPlatformApplication;
FORWARD_DECLARE_OBJECT_PTR(PlatformApplication);