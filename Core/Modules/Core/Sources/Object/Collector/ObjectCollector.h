#pragma once

#include "Templates/Map.h"

#include "Threading/CriticalSection.h"

class CObject;
class SCollector;
class CObjectLink;

namespace Objects::Properties
{
	class CProperty;
}

class CObjectCollector
{
	using CProperty = Objects::Properties::CProperty;

public:
	CObjectCollector();
    ~CObjectCollector();
    
	SCollector* Collector() const;
	void PushCollector(SCollector* Collector);
	void PopCollector();
	
	CObjectLink* AddObjectLink(CObject* Obj, CProperty* Property);
	void RemoveObjectLink(CObjectLink* Link);
	
	bool HasLinks(CObject* Obj) const;
	TArray<CObjectLink*> LinksForObject(CObject* Obj) const;
	
	void WatchObject(CObject* Obj);
	void UnWatchObject(CObject* Obj);

	void AddToRoot(CObject* Obj);
	void RemoveFromRoot(CObject* Obj);
	
	SizeT AliveObjectCount() const;

	void CollectGarbage();

	void SetQueuedForDestruction(CObject* Obj, bool bEnqueue);
	
private:
	SCriticalSection _objectsCriticalSection;
	TArray<CObject*> _globalObjects;
	TArray<CObject*> _rootedObjects;
	mutable TMap<CObject*, TArray<CObjectLink*>> _objectLinksMap;

	SCriticalSection _destructionQueueCriticalSection;
	TArray<CObject*> _destructionQueue;
	
	SCriticalSection _collectorCriticalSection;
	class SCollector* _currentCollector;

	void DestroyQueued();
	void DestroyObject(CObject* Obj);

	void RecursivelyMarkObjects(CObject* InRootObject, TArray<CObject*>& RefMarked);
};

extern CObjectCollector& GObjectCollector;
