#include "QueueProperty.h"

#include "Object/Collector/ObjectCollector.h"

namespace Objects::Properties
{
    CQueueProperty::CQueueProperty(CObject* InParent)
    :   Super(InParent)
    {
        //
    }

    void CQueueProperty::ObjectAdded(CObject* InObject)
    {
        _objectLinkMap.Add(InObject, GObjectCollector.AddObjectLink(InObject, this));
    }

    void CQueueProperty::ObjectRemoved(CObject* InObject)
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

    CObjectLink* CQueueProperty::LinkForObject(CObject* InObject) const
    {
        return _objectLinkMap[InObject];
    }
}