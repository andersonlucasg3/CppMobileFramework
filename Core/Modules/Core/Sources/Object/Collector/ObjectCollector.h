#pragma once

#include "Templates/Set.h"
#include "Templates/Map.h"

#include "Threading/Atomic.h"
#include "Threading/CriticalSection.h"

class CObject;
class CObjectLink;
class CReferencer;
class CThread;

struct SCollector;
struct SObjectCollectedListenerHandle;

class CObjectCollector
{
public:
	CORE_API CObjectCollector() = default;
    CORE_API ~CObjectCollector();
    
	CORE_API SCollector* Collector() const;
	CORE_API void PushCollector(SCollector* Collector);
	CORE_API void PopCollector();
	
	CORE_API CObjectLink* AddObjectLink(CObject* Obj, CReferencer* Referencer);
	CORE_API void RemoveObjectLink(CObjectLink* Link);

	CORE_API const SObjectCollectedListenerHandle& AddOnObjectCollectedListener(CObject* Object, const TFunction<void()>& OnCollected);
	CORE_API void RemoveOnObjectCollectedListener(CObject* Object, SObjectCollectedListenerHandle& Handle);
	
	CORE_API bool HasLinks(CObject* Obj) const;
	CORE_API TArray<CObjectLink*> LinksForObject(CObject* Obj) const;
	
	CORE_API void WatchObject(CObject* Obj);
	CORE_API void UnWatchObject(CObject* Obj);

	CORE_API void AddToRoot(CObject* Obj);
	CORE_API void RemoveFromRoot(CObject* Obj);
	
	CORE_API SizeT AliveObjectCount() const;

	CORE_API void SetQueuedForDestruction(CObject* Obj, bool bEnqueue);

	CORE_API void ForceCollectGarbage();
	
private:
	SCriticalSection _objectsCriticalSection;
	TArray<CObject*> _globalObjects;
	TArray<CObject*> _rootedObjects;
	mutable TMap<CObject*, TArray<CObjectLink*>> _objectLinksMap;

	SCriticalSection _destructionQueueCriticalSection;
	TSet<CObject*> _destructionQueue;

	SCriticalSection _listenersCriticalSection;
	TMap<CObject*, TMap<SObjectCollectedListenerHandle, TFunction<void()>>> _collectedListeners;
	
	TAtomic<bool> _bIsGarbageCollecting = false;

	// TODO: need a logic to schedule periodic collections on the main thread
	void CollectGarbage();

	void DestroyQueued();
	void DestroyObject(CObject* Obj);

	void RecursivelyMarkObjects(CObject* InFirstObject, CObject* InCurrentObject, TSet<CObject*>& RefMarked);
};

CORE_API extern CObjectCollector& GObjectCollector;
