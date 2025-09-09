#pragma once

class CObject;
class CReferencer;

class CORE_API CObjectLink
{
public:
	CObjectLink(CReferencer* InReferencer, CObject* InObject);
	~CObjectLink() = default;
	
	CObject* Object() const;
	CObject* Parent() const;
	CReferencer* Referencer() const;
	
private:
	CObject* _object = nullptr;
	CObject* _parent = nullptr;
	CReferencer* _referencer = nullptr;
};
