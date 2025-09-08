#pragma once

#include "Templates/Set.h"
#include "Templates/Map.h"

#include "Threading/CriticalSection.h"

class CObject;
class CObjectLink;

struct SCollector;

namespace Objects::Properties
{
	class CProperty;
}

class CObjectCollector
{
	using CProperty = Objects::Properties::CProperty;

public:
	CORE_API CObjectCollector();
    CORE_API ~CObjectCollector();
    
	CORE_API SCollector* Collector() const;
	CORE_API void PushCollector(SCollector* Collector);
	CORE_API void PopCollector();
	
	CORE_API CObjectLink* AddObjectLink(CObject* Obj, CProperty* Property);
	CORE_API void RemoveObjectLink(CObjectLink* Link);
	
	CORE_API bool HasLinks(CObject* Obj) const;
	CORE_API TArray<CObjectLink*> LinksForObject(CObject* Obj) const;
	
	CORE_API void WatchObject(CObject* Obj);
	CORE_API void UnWatchObject(CObject* Obj);

	CORE_API void AddToRoot(CObject* Obj);
	CORE_API void RemoveFromRoot(CObject* Obj);
	
	CORE_API SizeT AliveObjectCount() const;

	CORE_API void CollectGarbage();

	CORE_API CORE_API void SetQueuedForDestruction(CObject* Obj, bool bEnqueue);
	
private:
	SCriticalSection _objectsCriticalSection;
	TArray<CObject*> _globalObjects;
	TArray<CObject*> _rootedObjects;
	mutable TMap<CObject*, TArray<CObjectLink*>> _objectLinksMap;

	SCriticalSection _destructionQueueCriticalSection;
	TSet<CObject*> _destructionQueue;
	
	SCriticalSection _collectorCriticalSection;
	SCollector* _currentCollector;

	void DestroyQueued();
	void DestroyObject(CObject* Obj);

	void RecursivelyMarkObjects(CObject* InFirstObject, CObject* InCurrentObject, TArray<CObject*>& RefMarked);
};

CORE_API extern CObjectCollector& GObjectCollector;
