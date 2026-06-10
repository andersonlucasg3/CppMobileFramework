#pragma once

#include "Logger/Logger.h"

class CMicrosoftLogger : public CLogger
{
	using Super = CLogger;

protected:
	CORE_API void WriteLogLine(const std::string& LogLine) const override;

public:
	CORE_API CMicrosoftLogger();
	CORE_API ~CMicrosoftLogger() override = default;
};

typedef CMicrosoftLogger CPlatformLogger;