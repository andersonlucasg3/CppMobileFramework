#pragma once

#include "Threading/ScopeLock.h"
#include "Threading/CriticalSection.h"

#include "Object/Collector/Referencer.h"
#include "Object/Collector/ObjectCollector.h"

class CObject;
class CObjectLink;

template<typename TObject = CObject>
class TObjectPtr : public CReferencer
{
    using Super = CReferencer;

public:
    inline TObjectPtr()
    :   Super(nullptr)
    {
        //
    }

    inline TObjectPtr(TObject* Object)
    :   Super(nullptr)
    {
        _object = Object;
        _link = GObjectCollector.AddObjectLink(_object, this);
    }

    inline TObjectPtr(const TObjectPtr& Other)
    :   Super(nullptr)
    {
        _object = Other._object;
        _link = GObjectCollector.AddObjectLink(_object, this);
    }

    inline ~TObjectPtr()
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _link = nullptr;
    }

    inline TObject* Get() const
    {
        SScopeLock Lock(_criticalSection);

        return _object;
    }

    inline TObjectPtr& operator=(TObject* NewObject)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _object = NewObject;
        _link = GObjectCollector.AddObjectLink(_object, this);

        return *this;
    }

    inline TObjectPtr& operator=(const TObjectPtr& Other)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _object = Other._object;
        _link = GObjectCollector.AddObjectLink(_object, this);

        return *this;
    }

    inline operator TObject*() const
    {
        return Get();
    }

    inline TObject* operator->() const
    {
        return Get();
    }

    inline bool IsValid() const
    {
        return Get() != nullptr;
    }

private:
    TObject* _object = nullptr;
    CObjectLink* _link = nullptr;

    SCriticalSection _criticalSection;

    inline void ReleaseLinks() override
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);
        _link = nullptr;
    }

    inline void EnumerateLinks(const TFunction<void(CObjectLink*)>& InFunc) const override
    {
        SScopeLock Lock(_criticalSection);
        
        if (_link != nullptr)
        {
            InFunc(_link);
        }
    }

    template<typename TOtherObject>
    friend class TWeakObjectPtr;
};
