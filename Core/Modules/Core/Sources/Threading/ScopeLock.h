#pragma once

#include "CriticalSection.h"

struct SScopeLock
{
    CORE_API SScopeLock(const SCriticalSection& Section);
    CORE_API ~SScopeLock();

private:
    const SCriticalSection& _section;
};