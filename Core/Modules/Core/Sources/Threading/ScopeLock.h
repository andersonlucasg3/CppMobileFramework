#pragma once

#include "CriticalSection.h"

struct SScopeLock
{
public:
    CORE_API SScopeLock(const SCriticalSection& Section, bool bTryLock = false);
    CORE_API ~SScopeLock();

private:
    const SCriticalSection& _section;

    mutable bool _bLocked = false;
};