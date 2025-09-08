#include "ObjectLink.h"

#include "Object/Properties/Property.h"

CObjectLink::CObjectLink(CProperty* InProperty, CObject* InObject)
:	_object(InObject)
,	_parent(InProperty->Parent())
,	_property(InProperty)
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

Objects::Properties::CProperty* CObjectLink::Property() const
{
	return _property;
}
