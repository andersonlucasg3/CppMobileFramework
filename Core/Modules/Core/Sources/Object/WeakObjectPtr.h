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
    inline TWeakObjectPtr()
    :   _object(nullptr)
    {
        //
    }

    inline TWeakObjectPtr(TObject* Object)
    :   _object(Object)
    {
        _handle = GObjectCollector.AddOnObjectCollectedListener(_object, [this]()
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr;
        });
    }

    inline TWeakObjectPtr(const TWeakObjectPtr& Weak)
    {
        _object = Weak;
        _handle = GObjectCollector.AddOnObjectCollectedListener(_object, [this]()
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr;
        });
    }

    template<typename TOtherObject = CObject>
    inline TWeakObjectPtr(const TObjectPtr<TOtherObject>& ObjectPtr)
    {
        static_assert(std::is_base_of_v<CObject, TOtherObject>);

        _object = static_cast<TOtherObject*>(ObjectPtr._object);
        _handle = GObjectCollector.AddOnObjectCollectedListener(_object, [this]
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr;
        });
    }

    inline ~TWeakObjectPtr()
    {
        GObjectCollector.RemoveOnObjectCollectedListener(_object, _handle);
        _object = nullptr;
    }

    inline TWeakObjectPtr& operator=(TObject* Object)
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

    inline TWeakObjectPtr& operator=(const TWeakObjectPtr& Weak)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveOnObjectCollectedListener(_object, _handle);

        _object = Weak._object;
        _handle = GObjectCollector.AddOnObjectCollectedListener(_object, [this]()
        {
            SScopeLock Lock(_criticalSection);

            _object = nullptr;
        });

        return *this;
    }

    inline bool IsValid() const
    {
        SScopeLock Lock(_criticalSection);

        return _object != nullptr && !_object->IsQueuedForDestruction();
    }

    inline TObject* Get() const
    {
        SScopeLock Lock(_criticalSection);

        return _object;
    }

private:
    TObject* _object;
    SObjectCollectedListenerHandle _handle;

    SCriticalSection _criticalSection;
};