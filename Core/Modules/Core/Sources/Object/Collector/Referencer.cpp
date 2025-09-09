#include "Referencer.h"

#include "Object/Object.h"

CReferencer::CReferencer(CObject* InParent)
: _parent(InParent)
{
    if (_parent != nullptr)
    {
        _parent->_referencers.Add(this);
    }
}

CReferencer::~CReferencer()
{
    if (_parent != nullptr)
    {
        _parent->_referencers.Remove(this);
    }
}

CObject* CReferencer::Parent() const
{
    return _parent;
}