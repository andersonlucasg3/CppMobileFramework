#pragma once

#include "Threading/ScopeLock.h"
#include "Threading/CriticalSection.h"

#include "Object/Collector/ObjectCollector.h"
#include "Object/Collector/ObjectCollectedListenerHandle.h"

class CObject;
template<typename TObject>
class TObjectPtr;

template<typename TObject = CObject>
class TWeakObjectPtr
{
public:
    TWeakObjectPtr()
    :   _object(nullptr)
    {
        //
    }

    TWeakObjectPtr(TObject* Object)
    :   _object(Object)
    {
        _handle = GObjectCollector.AddOnObjectCollectedListener(_object, [this]()
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr;
        });
    }

    TWeakObjectPtr(const TWeakObjectPtr& Weak)
    {
        _object = Weak;
        _handle = GObjectCollector.AddOnObjectCollectedListener(_object, [this]()
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr;
        });
    }

    template<typename TOtherObject = CObject>
    TWeakObjectPtr(const TObjectPtr<TOtherObject>& ObjectPtr)
    {
        _object = ObjectPtr;
        _handle = GObjectCollector.AddOnObjectCollectedListener(_object, [this]
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr;
        });
    }

    ~TWeakObjectPtr()
    {
        GObjectCollector.RemoveOnObjectCollectedListener(_object, _handle);
        _object = nullptr;
    }

    TWeakObjectPtr& operator=(TObject* Object)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveOnObjectCollectedListener(_object, _handle);

        _object = Object;
        _handle = GObjectCollector.AddOnObjectCollectedListener(Object, [this]()
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr; 
        });

        return *this;
    }

    TWeakObjectPtr& operator=(const TWeakObjectPtr& Weak)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveOnObjectCollectedListener(_object, _handle);

        _object = Weak;
        _handle = GObjectCollector.AddOnObjectCollectedListener(_object, [this]()
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr;
        });

        return *this;
    }

    bool IsValid()
    {
        SScopeLock Lock(_criticalSection);

        return _object != nullptr && !_object->IsQueuedForDestruction();
    }

    TObject* Get()
    {
        SScopeLock Lock(_criticalSection);

        return _object;
    }

private:
    TObject* _object;
    SObjectCollectedListenerHandle _handle;

    SCriticalSection _criticalSection;
};