#pragma once

class CObject;

namespace Objects::Properties
{
	class CProperty;
}

class CObjectLink
{
	using CProperty = Objects::Properties::CProperty;

public:
	CObjectLink(CProperty* InProperty, CObject* InObject);
	~CObjectLink() = default;
	
	CObject* Object() const;
	CObject* Parent() const;
	CProperty* Property() const;
	
private:
	CObject* _object;
	CObject* _parent;
	CProperty* _property;
};
