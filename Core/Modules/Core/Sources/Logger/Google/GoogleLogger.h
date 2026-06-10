#pragma once

#include "Logger/Logger.h"

class CString;

class CGoogleLogger : public CLogger
{
    using Super = CLogger;

protected:
    CORE_API void WriteLogLine(const CString& LogLine) const override;

public:
    CORE_API CGoogleLogger() = default;
    CORE_API ~CGoogleLogger() override = default;
};

typedef CGoogleLogger CPlatformLogger;