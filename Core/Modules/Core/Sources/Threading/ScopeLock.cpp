#include "ScopeLock.h"

SScopeLock::SScopeLock(const SCriticalSection& Section) 
:   _section(Section)
{
    _section.Lock();
}

SScopeLock::~SScopeLock()
{
    _section.Unlock();
}