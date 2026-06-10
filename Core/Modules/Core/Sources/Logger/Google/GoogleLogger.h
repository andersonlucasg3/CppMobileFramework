#pragma once

#include "Logger/Logger.h"

class CString;

class CAndroidLogger : public CLogger
{
    using Super = CLogger;

protected:
    CORE_API void WriteLogLine(const CString& LogLine) const override;

public:
    CORE_API CAndroidLogger() = default;
    CORE_API ~CAndroidLogger() override = default;
};

typedef CAndroidLogger CPlatformLogger;