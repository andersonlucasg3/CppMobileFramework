#pragma once

class CObject;
class CReferencer;

class CObjectLink
{
public:
	CObjectLink(CReferencer* InReferencer, CObject* InObject);
	~CObjectLink() = default;
	
	CObject* Object() const;
	CObject* Parent() const;
	CReferencer* Referencer() const;
	
private:
	CObject* _object;
	CObject* _parent;
	CReferencer* _referencer;
};
