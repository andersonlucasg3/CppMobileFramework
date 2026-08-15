#include "GoogleLogger.h"

#include <android/log.h>

void CGoogleLogger::WriteLogLine(const std::string& LogLine) const
{
    Super::WriteLogLine(LogLine);

    android_LogPriority Priority = ANDROID_LOG_INFO;

    if (LogLine.starts_with("ERROR:"))
    {
        Priority = ANDROID_LOG_ERROR;
    }
    else if (LogLine.starts_with("FATAL:"))
    {
        Priority = ANDROID_LOG_FATAL;
    }
    else if (LogLine.starts_with("WARN:"))
    {
        Priority = ANDROID_LOG_WARN;
    }

    __android_log_print(Priority, "CppMobileFramework", "%s", LogLine.c_str());
}