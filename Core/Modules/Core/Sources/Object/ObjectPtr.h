#pragma once

#include "Object/Collector/Referencer.h"
#include "Object/Object.h"
#include "Object/Properties/Property.h"
#include "Object/Collector/ObjectCollector.h"
#include "Threading/CriticalSection.h"
#include "Threading/ScopeLock.h"

#include <cstddef>

template<typename TObject = CObject>
class TObjectPtr : public CReferencer
{
    using Super = CReferencer;

public:
    TObjectPtr()
    :   Super(nullptr)
    ,   _object(nullptr)
    {
        //
    }

    TObjectPtr(TObject* Object)
    :   Super(nullptr)
    ,   _object(Object)
    ,   _link(GObjectCollector.AddObjectLink(_object, this))
    {
        //
    }

    TObjectPtr(const TObjectPtr& Other)
    :   Super(nullptr)
    ,   _object(Other._object)
    ,   _link(GObjectCollector.AddObjectLink(_object, this))
    {
        //
    }

    ~TObjectPtr()
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _link = nullptr;
    }

    TObject* Get()
    {
        SScopeLock Lock(_criticalSection);

        return _object;
    }

    TObjectPtr& operator=(CObject* NewObject)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _object = NewObject;
        _link = GObjectCollector.AddObjectLink(_object, this);
    }

    TObjectPtr& operator=(const TObjectPtr& Other)
    {
        SScopeLock Lock(_criticalSection);

        GObjectCollector.RemoveObjectLink(_link);

        _object = Other._object;
        _link = GObjectCollector.AddObjectLink(_object, this);
    }

    operator TObject*()
    {
        SScopeLock Lock(_criticalSection);

        return _object;
    }

    TObject* operator->()
    {
        SScopeLock Lock(_criticalSection);

        return _object;
    }

private:
    TObject* _object;
    CObjectLink* _link;

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
};