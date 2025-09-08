#pragma once

#include "Templates/Array.h"

class CObject;

struct SCollector
{
public:
	SCollector() = default;
	virtual ~SCollector() = default;
	
	void WatchObject(CObject* Obj);
	void UnWatchObject(CObject* Obj);
	
	void SetParent(SCollector* Parent);
	SCollector* Parent() const;

	const TArray<CObject*>& WatchedObjects() const;
	
private:
	SCollector* _parent = nullptr;

	TArray<CObject*> _watching;
};
