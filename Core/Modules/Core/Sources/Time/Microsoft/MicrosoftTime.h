#pragma once

#include "Time/Time.h"

class CWindowsTime : public CTime
{
public:
	CORE_API ~CWindowsTime() override = default;

	CORE_API Double GetTimeInSeconds() const override;
};

typedef CWindowsTime CPlatformTime;