#include "Object.h"

#include "Collector/ObjectCollector.h"
#include "Threading/ScopeLock.h"

CObject::CObject()
{
    GObjectCollector.WatchObject(this);
}

CObject::~CObject()
{
    
}

bool CObject::IsRooted() const
{
    SScopeLock Lock(_criticalSection);

    return _bIsRooted;
}

void CObject::MakeRooted()
{
    SScopeLock Lock(_criticalSection);

    _bIsRooted = true;

    GObjectCollector.AddToRoot(this);
}

void CObject::MakeUnrooted()
{
    SScopeLock Lock(_criticalSection);

    _bIsRooted = false;

    GObjectCollector.RemoveFromRoot(this);
}

bool CObject::IsQueuedForDestruction() const
{
    SScopeLock Lock(_criticalSection);

    return _bIsQueuedForDestruction;
}

void CObject::SetQueuedForDestruction(bool bIsQueuedForDestruction)
{
    SScopeLock Lock(_criticalSection);

    if (_bIsQueuedForDestruction != bIsQueuedForDestruction)
    {
        _bIsQueuedForDestruction = bIsQueuedForDestruction;

        GObjectCollector.SetQueuedForDestruction(this, bIsQueuedForDestruction);
    }
}