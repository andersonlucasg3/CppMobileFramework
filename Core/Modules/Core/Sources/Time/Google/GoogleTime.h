#pragma once

#include "Time/Time.h"

class CAndroidTime : public CTime
{
public:
    CORE_API ~CAndroidTime() override = default;

    CORE_API Double GetTimeInSeconds() const override;
};

typedef CAndroidTime CPlatformTime;
