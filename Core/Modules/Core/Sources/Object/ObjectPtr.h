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
    TObjectPtr()
    :   Super(nullptr)
    {
        //
    }

    TObjectPtr(TObject* Object)
    :   Super(nullptr)
    {
        _object = Object;
        _link = GObjectCollector.AddObjectLink(_object, this);
    }

    TObjectPtr(const TObjectPtr& Other)
    :   Super(nullptr)
    {
        _object = Other._object;
        _link = GObjectCollector.AddObjectLink(_object, this);
    }

    ~TObjectPtr()
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _link = nullptr;
    }

    TObject* Get() const
    {
        SScopeLock Lock(_criticalSection);

        return _object;
    }

    TObjectPtr& operator=(TObject* NewObject)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _object = NewObject;
        _link = GObjectCollector.AddObjectLink(_object, this);

        return *this;
    }

    TObjectPtr& operator=(const TObjectPtr& Other)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _object = Other._object;
        _link = GObjectCollector.AddObjectLink(_object, this);

        return *this;
    }

    operator TObject*() const
    {
        return Get();
    }

    TObject* operator->() const
    {
        return Get();
    }

private:
    TObject* _object = nullptr;
    CObjectLink* _link = nullptr;

    SCriticalSection _criticalSection;

    void ReleaseLinks() override
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);
        _link = nullptr;
    }

    void EnumerateLinks(const TFunction<void(CObjectLink*)>& InFunc) const override
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
