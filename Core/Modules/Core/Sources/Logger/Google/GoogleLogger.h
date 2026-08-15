#pragma once

#include "Logger/Logger.h"

class CGoogleLogger : public CLogger
{
    using Super = CLogger;

protected:
    CORE_API void WriteLogLine(const std::string& LogLine) const override;

public:
    CORE_API CGoogleLogger() = default;
    CORE_API ~CGoogleLogger() override = default;
};

typedef CGoogleLogger CPlatformLogger;