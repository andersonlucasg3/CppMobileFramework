#pragma once

#include "Time/Time.h"

class CMicrosoftTime : public CTime
{
public:
	CORE_API ~CMicrosoftTime() override = default;

	CORE_API Double GetTimeInSeconds() const override;
};

typedef CMicrosoftTime CPlatformTime;