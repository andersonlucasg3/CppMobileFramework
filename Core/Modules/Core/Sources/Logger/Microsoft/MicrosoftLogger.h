#pragma once

#include "Logger/Logger.h"

class CWindowsLogger : public CLogger
{
	using Super = CLogger;

protected:
	CORE_API void WriteLogLine(const std::string& LogLine) const override;

public:
	CORE_API CWindowsLogger();
	CORE_API ~CWindowsLogger() override = default;
};

typedef CWindowsLogger CPlatformLogger;