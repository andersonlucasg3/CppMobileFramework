#pragma once

#include "Object/Collector/Referencer.h"

#include "Templates/Map.h"
#include "Templates/Queue.h"

#include "Threading/ScopeLock.h"
#include "Threading/CriticalSection.h"

#include "Object/Collector/ObjectLink.h"

#include <type_traits>

class CObject;

namespace Objects::Properties
{
    class CQueueProperty : public CReferencer
    {
        using Super = CReferencer;

    public:
        CORE_API CQueueProperty(CObject* Parent);
        CORE_API virtual ~CQueueProperty() override = default;

    protected:
        CORE_API void ObjectAdded(CObject* InObject);
        CORE_API void ObjectRemoved(CObject* InObject);

        CORE_API CObjectLink* LinkForObject(CObject* InObject) const;

    private:
        mutable TMap<CObject*, CObjectLink*> _objectLinkMap;
    };

    template<typename TObject = CObject>
    class TQueueProperty : public CQueueProperty
    {
        static_assert(std::is_base_of_v<CObject, TObject>);

        using Super = CQueueProperty;

    public:
        inline TQueueProperty(CObject* InParent)
        :   Super(InParent)
        {
            //
        }

        ~TQueueProperty() override = default;

        inline void Enqueue(TObject* InObject)
        {
            SScopeLock Lock(_criticalSection);

            _objects.Enqueue(InObject);

            Super::ObjectAdded(InObject);
        }

        inline bool Dequeue(TObject*& OutObject)
        {
            SScopeLock Lock(_criticalSection);

            if (_objects.Dequeue(OutObject))
            {
                Super::ObjectRemoved(OutObject);

                return true;
            }

            return false;
        }

        inline bool Peak(TObject*& OutObject)
        {
            SScopeLock Lock(_criticalSection);

            return _objects.Peak(OutObject);
        }

        inline bool IsEmpty() const
        {
            SScopeLock Lock(_criticalSection);

            return _objects.IsEmpty();
        }

    private:
        TQueue<TObject*> _objects;

        SCriticalSection _criticalSection;

        inline void ReleaseLinks() override
        {
            SScopeLock Lock(_criticalSection);

            _objects.ForEach([this](TObject* Object)
            {
                Super::ObjectRemoved(Object);
            });
            
            _objects.RemoveAll();
        }

        inline void EnumerateLinks(const TFunction<void(CObjectLink*)>& InFunc) const override 
        {
            SScopeLock Lock(_criticalSection);

            _objects.ForEach([this, &InFunc](TObject* Object)
            {
                CObjectLink* Link = Super::LinkForObject(Object);

                InFunc(Link);
            });
        }
    };
}

#define QUEUEPROPERTY(ElementName, Name) Objects::Properties::TQueueProperty< ElementName > Name = this