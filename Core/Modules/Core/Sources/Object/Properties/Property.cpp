#include "Property.h"

#include "Object/Object.h"

namespace Objects::Properties
{
	CProperty::CProperty(CObject* InParent)
	: _parent(InParent)
	{
		if (_parent != nullptr)
		{
			_parent->_properties.Add(this);
		}
	}

	CProperty::~CProperty()
	{
		if (_parent != nullptr)
		{
			_parent->_properties.Remove(this);
		}
	}

	CObject* CProperty::Parent() const
	{
		return _parent;
	}
}