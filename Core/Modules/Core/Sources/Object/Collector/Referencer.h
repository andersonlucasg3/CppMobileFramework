#pragma once

#include "Templates/Functions.h"

class CObject;
class CObjectLink;

class CReferencer
{
public:
    CORE_API explicit CReferencer(CObject* InParent);
    CORE_API virtual ~CReferencer();
    
    CORE_API CObject* Parent() const;
    
protected:
    CORE_API virtual void ReleaseLinks() = 0;
    CORE_API virtual void EnumerateLinks(const TFunction<void(CObjectLink*)>& InFunc) const = 0;

private:
    CObject* _parent;
    
    friend class CObjectCollector;
};