#pragma once

#include "CriticalSection.h"

struct SScopeLock
{
public:
    CORE_API SScopeLock(const SCriticalSection& Section);
    CORE_API ~SScopeLock();

private:
    const SCriticalSection& _section;
};