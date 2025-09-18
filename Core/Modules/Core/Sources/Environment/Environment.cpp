#include "Environment.h"

#include "Preprocessors.h"

#include COMPILE_PLATFORM_HEADER(Environment.h)

static const CPlatformEnvironment GPlatformEnvironment;
const CEnvironment& GEnvironment = GPlatformEnvironment;
