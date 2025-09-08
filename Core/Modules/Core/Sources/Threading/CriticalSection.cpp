#include "CriticalSection.h"

void SCriticalSection::Lock() const
{
    _mutex.lock();
}

bool SCriticalSection::TryLock() const
{
    return _mutex.try_lock();
}

void SCriticalSection::Unlock() const
{
    _mutex.unlock();
}