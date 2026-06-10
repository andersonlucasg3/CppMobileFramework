#include "Memory.h"

#include "Preprocessors.h"

#include COMPILE_PLATFORM_GROUP_HEADER(Memory.h)

static CPlatformMemory GPlatformMemory;
const CMemory& GMemory = GPlatformMemory;