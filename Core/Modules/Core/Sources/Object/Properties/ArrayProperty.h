#pragma once

#include "Object/Collector/Referencer.h"

#include "Templates/Map.h"

#include "Threading/ScopeLock.h"
#include "Threading/CriticalSection.h"

#include "Object/Collector/ObjectLink.h"

class CObject;

namespace Objects::Properties
{
    class CArrayProperty : public CReferencer
    {
        using Super = CReferencer;

    public:
        CORE_API CArrayProperty(CObject* InParent);
        CORE_API virtual ~CArrayProperty() override = default;

    protected:
        CORE_API void ObjectAdded(CObject* InObject);
        CORE_API void ObjectRemoved(CObject* InObject);

        CORE_API CObjectLink* LinkForObject(CObject* InObject) const;

    private:
        mutable TMap<CObject*, CObjectLink*> _objectLinkMap;
    };

    template<typename TObject = CObject>
    class TArrayProperty : public CArrayProperty
    {
        using Super = CArrayProperty;

    public:
        inline TArrayProperty(CObject* InParent)
        :   Super(InParent)
        {
            //
        }

        ~TArrayProperty() override = default;

        inline void Add(TObject* InObject) 
        {
            SScopeLock Lock(_criticalSection);

            _objects.Add(InObject);

            Super::ObjectAdded(InObject);
        }

        inline void Remove(TObject* InObject)
        {
            SScopeLock Lock(_criticalSection);

            if (_objects.Remove(InObject))
            {
                Super::ObjectRemoved(InObject);
            }
        }

        inline TObject* operator[](SizeT Index)
        {
            SScopeLock Lock(_criticalSection);

            return _objects[Index];
        }

    private:
        TArray<TObject*> _objects;

        SCriticalSection _criticalSection;

        inline void ReleaseLinks() override
        {
            SScopeLock Lock(_criticalSection);

            for (TObject* Object : _objects)
            {
                Super::ObjectRemoved(Object);
            }

            _objects.RemoveAll();
        }

        inline void EnumerateLinks(const TFunction<void(CObjectLink*)>& InFunc) const override 
        {
            SScopeLock Lock(_criticalSection);

            for (TObject* Object : _objects)
            {
                CObjectLink* Link = Super::LinkForObject(Object);

                InFunc(Link);
            }
        }
    };
}

#define ARRAYPROPERTY(ElementName, Name) Objects::Properties::TArrayProperty< ElementName > Name = this