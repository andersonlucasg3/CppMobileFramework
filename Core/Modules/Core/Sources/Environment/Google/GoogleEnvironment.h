#pragma once

#include "Environment/Environment.h"

class CGoogleEnvironment : public CEnvironment
{
public:
    CORE_API CGoogleEnvironment() = default;
    CORE_API ~CGoogleEnvironment() override = default;

    CORE_API const char* NewLine() const override;
};

typedef CGoogleEnvironment CPlatformEnvironment;