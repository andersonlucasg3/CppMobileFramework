#pragma once

#if PLATFORM_WINDOWS

#include "Environment/Environment.h"

class CWindowsEnvironment : public CEnvironment
{
	using Super = CEnvironment;

public:
	CORE_API ~CWindowsEnvironment() override = default;

	CORE_API const char* NewLine() const override;
};

typedef CWindowsEnvironment CPlatformEnvironment;

#endif