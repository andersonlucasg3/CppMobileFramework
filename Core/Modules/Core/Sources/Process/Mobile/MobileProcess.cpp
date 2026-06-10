#include "MobileProcess.h"

#include "Preprocessors.h"

#include COMPILE_PLATFORM_GROUP_HEADER_FEATURE(Process, Process.h)

static CPlatformProcess GPlatformProcess;
CMobileProcess& GMobileProcess = GPlatformProcess;
CProcess& GProcess = GMobileProcess;