#pragma once

#include "Environment/Environment.h"

class CAppleEnvironment : public CEnvironment
{
public:
    CORE_API ~CAppleEnvironment() override = default;

    CORE_API const char* NewLine() const override;
};

typedef CAppleEnvironment CPlatformEnvironment;