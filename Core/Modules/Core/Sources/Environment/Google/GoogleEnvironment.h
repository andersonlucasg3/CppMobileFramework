#pragma once

#include "Environment/Environment.h"

class CAndroidEnvironment : public CEnvironment
{
public:
    CORE_API CAndroidEnvironment() = default;
    CORE_API ~CAndroidEnvironment() override = default;

    CORE_API const char* NewLine() const override;
};

typedef CAndroidEnvironment CPlatformEnvironment;