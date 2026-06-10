#pragma once

#include "Time/Time.h"

class CGoogleTime : public CTime
{
public:
    CORE_API ~CGoogleTime() override = default;

    CORE_API Double GetTimeInSeconds() const override;
};

typedef CGoogleTime CPlatformTime;
