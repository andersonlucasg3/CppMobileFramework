#pragma once

#include "Application.h"
#include "Object/ClassMacros.h"

class CIOSApplication : public CApplication
{
    using Super = CApplication;

public:
    APPLICATION_API CIOSApplication() = default;
    APPLICATION_API ~CIOSApplication() override = default;

    APPLICATION_API int Run(int argc, char* argv[]) override;
};

typedef CIOSApplication CPlatformApplication;
FORWARD_DECLARE_OBJECT_PTR(PlatformApplication);