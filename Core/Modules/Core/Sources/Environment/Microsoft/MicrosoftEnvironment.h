#pragma once

#if PLATFORM_WINDOWS

#include "Environment/Environment.h"

class CMicrosoftEnvironment : public CEnvironment
{
	using Super = CEnvironment;

public:
	CORE_API ~CMicrosoftEnvironment() override = default;

	CORE_API const char* NewLine() const override;
};

typedef CMicrosoftEnvironment CPlatformEnvironment;

#endif