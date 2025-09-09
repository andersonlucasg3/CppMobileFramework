#include "ObjectLink.h"

#include "Object/Object.h"

#include "Referencer.h"

CObjectLink::CObjectLink(CReferencer* InReferencer, CObject* InObject)
:	_object(InObject)
,	_parent(InReferencer->Parent())
,	_referencer(InReferencer)
{
	
}

CObject* CObjectLink::Object() const
{
	return _object;
}

CObject* CObjectLink::Parent() const
{
	return _parent;
}

CReferencer* CObjectLink::Referencer() const
{
	return _referencer;
}
