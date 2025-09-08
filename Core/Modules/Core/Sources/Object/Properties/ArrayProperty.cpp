#include "ArrayProperty.h"

#include "Templates/Map.h"

#include "Object/Object.h"
#include "Object/Collector/ObjectLink.h"

#include <stdexcept>

namespace Objects::Properties
{
    CArrayProperty::CArrayProperty(CObject* InParent)
    :   Super(InParent)
    {
        //
    }

    void CArrayProperty::ObjectAdded(CObject* InObject)
    {
        _objectLinkMap.Add(InObject, GObjectCollector.AddObjectLink(InObject, this));
    }

    void CArrayProperty::ObjectRemoved(CObject* InObject)
    {
        CObjectLink* Link = nullptr;
        if (_objectLinkMap.RemoveAndCopyValue(InObject, Link) && Link != nullptr)
        {
            GObjectCollector.RemoveObjectLink(Link);
        }
        else
        {
            throw std::runtime_error("removed element with no link?");
        }
    }

    CObjectLink* CArrayProperty::LinkForObject(CObject* InObject) const
    {
        return _objectLinkMap[InObject];
    }
}