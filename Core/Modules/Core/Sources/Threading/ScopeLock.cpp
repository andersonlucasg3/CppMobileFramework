#include "ScopeLock.h"

SScopeLock::SScopeLock(const SCriticalSection& Section)
:   _section(Section)
{
    if (!_section.TryLock())
    {
        _section.Lock();
    }
}

SScopeLock::~SScopeLock()
{
    _section.Unlock();
}