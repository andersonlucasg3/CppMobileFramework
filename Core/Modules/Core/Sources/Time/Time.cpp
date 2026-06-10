#include "Time.h"

#include "Preprocessors.h"

#include COMPILE_PLATFORM_GROUP_HEADER(Time.h)

static CPlatformTime GPlatformTime;
const CTime& GTime = GPlatformTime; 
