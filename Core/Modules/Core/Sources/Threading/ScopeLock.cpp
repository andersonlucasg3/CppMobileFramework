#include "ScopeLock.h"

SScopeLock::SScopeLock(const SCriticalSection& Section, bool bTryLock)
:   _section(Section)
{
    if (bTryLock)
    {
        _bLocked = _section.TryLock();
    }
    else
    {
        _section.Lock();
        _bLocked = true;
    }
}

SScopeLock::~SScopeLock()
{
    if (_bLocked)
    {
        _section.Unlock();
    }
}