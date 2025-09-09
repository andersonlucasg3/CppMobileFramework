#include "ObjectCollector.h"

#include "Defines/Types.h"

#include "Templates/Map.h"
#include "Templates/Array.h"
#include "Templates/Functions.h"

#include "Threading/Thread.h"
#include "Threading/ScopeLock.h"

#include "Collector.h"
#include "ObjectLink.h"

#include "Object/Object.h"
#include "Object/Collector/Referencer.h"
#include "Object/Collector/ObjectCollectedListenerHandle.h"

#include <stdexcept>

static CObjectCollector ObjectCollector;
CObjectCollector& GObjectCollector = ObjectCollector;

CObjectCollector::CObjectCollector()
:   _currentCollector(nullptr)
{
    
}

CObjectCollector::~CObjectCollector()
{
	// delete all pending objects
	TArray<CObject*> RootedCopy = _rootedObjects;
	for (CObject* Rooted : RootedCopy)
	{
		Rooted->MakeUnrooted();
	}

	if (_bHasStartedCollecting)
	{
		_bHasStartedCollecting = false; // kills the thread
		_collectGarbageSemaphore.NotifyOne(); // release the semaphore
		_garbageCollectorThread->Join(); // wait the thread to finish
	}

	CollectGarbage(); // collect last garbage
}

void CObjectCollector::StartCollecting(UInt64 IntervalInMillis)
{
	if (_bHasStartedCollecting.CompareExchange(true)) return;

	_garbageCollectorThread = CThread::Create();
	_garbageCollectorThread->MakeRooted();

	_garbageCollectorThread->Start([this, IntervalInMillis](CThread* Thread)
	{
		while (_bHasStartedCollecting)
		{
			_collectGarbageSemaphore.Wait(IntervalInMillis);

			if (!_bHasStartedCollecting) return; // avoid the last GC here

			CollectGarbage();

			Thread->Sleep(100);
		}
	});
}

bool CObjectCollector::HasStartedCollecting() const
{
	return _bHasStartedCollecting;
}

SCollector* CObjectCollector::Collector() const
{
	SScopeLock Lock(_collectorCriticalSection);

	return _currentCollector;
}

void CObjectCollector::PushCollector(SCollector *Collector)
{
	SScopeLock Lock(_collectorCriticalSection);

	if (_currentCollector != nullptr)
	{
		Collector->SetParent(_currentCollector);
	}
	
	_currentCollector = Collector;
}

void CObjectCollector::PopCollector()
{
	SScopeLock Lock(_collectorCriticalSection);

	if (_currentCollector == nullptr)
	{
		return;
	}

	// copy
	TArray<CObject*> WatchedObjects = _currentCollector->WatchedObjects();

	SCollector* Parent = _currentCollector->Parent();
	_currentCollector = nullptr;
	
	for (CObject* Obj : WatchedObjects)
	{
		if (!HasLinks(Obj))
		{
			Obj->SetQueuedForDestruction(true);
		}
		else if (Parent != nullptr)
		{
			// Tell the parent to keep an eye in this object
			Parent->WatchObject(Obj);
		}
	}
	
	_currentCollector = Parent;

	DestroyQueued();	
}

CObjectLink* CObjectCollector::AddObjectLink(CObject* Obj, CReferencer* Referencer)
{
	if (Obj == nullptr) return nullptr;
	
	SScopeLock Lock(_objectsCriticalSection, true);

	if (Obj->IsQueuedForDestruction())
	{
		Obj->SetQueuedForDestruction(false);
	}
	
	CObjectLink* Link = new CObjectLink(Referencer, Obj);
	
	TArray<CObjectLink*>* LinksArray = _objectLinksMap.Find(Obj);
	if (LinksArray == nullptr)
	{
		LinksArray = &_objectLinksMap.Add(Obj, TArray<CObjectLink*>()).Value;
	}

	LinksArray->Add(Link);
	
	return Link;
}

void CObjectCollector::RemoveObjectLink(CObjectLink* Link)
{
	if (Link == nullptr) return;

	SScopeLock Lock(_objectsCriticalSection, true);
	
	CObject* Object = Link->Object();
	
	if (TArray<CObjectLink*>* LinksArray = _objectLinksMap.Find(Object))
	{
		LinksArray->Remove(Link);
			
		delete Link;
	}

	if (!HasLinks(Object))
	{
		Object->SetQueuedForDestruction(true);
	}
}

const SObjectCollectedListenerHandle& CObjectCollector::AddOnObjectCollectedListener(CObject* Object, const TFunction<void()>& OnCollected)
{
	static SObjectCollectedListenerHandle Invalid;
	if (Object == nullptr) return Invalid;
	
	SScopeLock Lock(_listenersCriticalSection);

	static UInt64 CurrentHandle = 0;

	TMap<SObjectCollectedListenerHandle, TFunction<void()>>* Listeners = _collectedListeners.Find(Object);
	if (Listeners == nullptr)
	{
		Listeners = &_collectedListeners.Add(Object, TMap<SObjectCollectedListenerHandle, TFunction<void()>>()).Value;
	}

	SObjectCollectedListenerHandle Handle = CurrentHandle++;
	return Listeners->Add(Handle, OnCollected).Key;
}

void CObjectCollector::RemoveOnObjectCollectedListener(CObject* Object, SObjectCollectedListenerHandle& Handle)
{
	if (Object == nullptr || !Handle.IsValid()) return;

	SScopeLock Lock(_listenersCriticalSection);

	if (TMap<SObjectCollectedListenerHandle, TFunction<void()>>* Listeners = _collectedListeners.Find(Object))
	{
		Listeners->Remove(Handle);
	}

	Handle.Invalidate();
}

bool CObjectCollector::HasLinks(CObject* Obj) const
{
	SScopeLock Lock(_objectsCriticalSection, true);

	if (TArray<CObjectLink*>* LinksArray = _objectLinksMap.Find(Obj))
	{
		return LinksArray->Num() > 0;
	}
	
	return false;
}

void CObjectCollector::WatchObject(CObject *Obj)
{
	SScopeLock Lock(_objectsCriticalSection, true);

	_globalObjects.Add(Obj);

	if (Obj->IsRooted()) return;
	
	if (SCollector* Collector = _currentCollector)
	{
		Collector->WatchObject(Obj);
	}
}

void CObjectCollector::UnWatchObject(CObject *Obj)
{
	SScopeLock Lock(_objectsCriticalSection, true);

	_globalObjects.Remove(Obj);

	if (Obj->IsRooted()) return;
	
	if (SCollector* Collector = _currentCollector)
	{
		Collector->UnWatchObject(Obj);
	}
}

void CObjectCollector::AddToRoot(CObject* Obj)
{
	SScopeLock Lock(_objectsCriticalSection, true);

	_rootedObjects.Add(Obj);

	if (_currentCollector != nullptr)
	{
		_currentCollector->UnWatchObject(Obj);
	}
}

void CObjectCollector::RemoveFromRoot(CObject* Obj)
{
	SScopeLock Lock(_objectsCriticalSection, true);

	_rootedObjects.Remove(Obj);

	if (_currentCollector != nullptr)
	{
		_currentCollector->WatchObject(Obj);
	}
}

SizeT CObjectCollector::AliveObjectCount() const
{
	SScopeLock Lock(_objectsCriticalSection, true);

	return _globalObjects.Num();
}

void CObjectCollector::SetQueuedForDestruction(CObject* Obj, bool bEnqueue)
{
	SScopeLock Lock(_destructionQueueCriticalSection);

	if (bEnqueue)
	{
		_destructionQueue.Insert(Obj);
	}
	else
	{
		_destructionQueue.Remove(Obj);
	}
}

void CObjectCollector::ForceCollectGarbage()
{
	if (_bIsGarbageCollecting) return;

	if (_bHasStartedCollecting)
	{
		_collectGarbageSemaphore.NotifyOne();
	}
	else
	{
		CollectGarbage(); // no thread call directly
	}
}

void CObjectCollector::CollectGarbage()
{
	if (_bIsGarbageCollecting.CompareExchange(true)) return;

	SScopeLock ObjectsLock(_objectsCriticalSection, true);

	static TArray<CObject*> Marked;
	
	for (CObject* Object : _rootedObjects)
	{
		RecursivelyMarkObjects(Object, Object, Marked);
	}

	for (CObject* Object : _globalObjects)
	{
		if (Object->IsRooted()) continue;

		if (!Marked.Contains(Object))
		{
			Object->SetQueuedForDestruction(true);
		}
	}

	DestroyQueued();

	Marked.RemoveAll(false);

	_bIsGarbageCollecting = false;
}

void CObjectCollector::DestroyQueued()
{
	SScopeLock DestructionLock(_destructionQueueCriticalSection);

	for (CObject* Object : _destructionQueue)
	{
		DestroyObject(Object);
	}

	_destructionQueue.RemoveAll();
}

void CObjectCollector::DestroyObject(CObject* Obj)
{
	SScopeLock ObjectsLock(_objectsCriticalSection, true);

	if (TArray<CObjectLink*>* LinksPtr = _objectLinksMap.Find(Obj))
	{
		TArray<CObjectLink*> LinksCopy = *LinksPtr;
		for (CObjectLink* Link : LinksCopy)
		{
			Link->Referencer()->ReleaseLinks();
		}
	}

	for (CReferencer* Referencer : Obj->_referencers)
	{
		Referencer->ReleaseLinks();
	}

	// do some index logic to fast removal
	_globalObjects.Remove(Obj);
	TArray<CObjectLink*> Links;
	_objectLinksMap.RemoveAndCopyValue(Obj, Links);

	if (Links.Num() > 0) throw new std::runtime_error("should not have any live links");

	{
		SScopeLock Lock(_listenersCriticalSection);
		TMap<SObjectCollectedListenerHandle, TFunction<void()>> Listeners;
		if (_collectedListeners.RemoveAndCopyValue(Obj, Listeners))
		{
			Listeners.ForEach([](const TKeyValuePair<SObjectCollectedListenerHandle, TFunction<void()>>& Pair)
			{
				Pair.Value();
			});
		}
	}

	delete Obj;
}

void CObjectCollector::RecursivelyMarkObjects(CObject* InFirstObject, CObject* InCurrentObject, TArray<CObject*>& RefMarked)
{
	TFunction<void(CObjectLink*)> LinkFunction = [this, InFirstObject, &RefMarked](CObjectLink* Link)
	{
		if (InFirstObject == Link->Object())
		{
			return;
		}

		this->RecursivelyMarkObjects(InFirstObject, Link->Object(), RefMarked);
	};

	RefMarked.Add(InCurrentObject);

	for (CReferencer* Referencer : InCurrentObject->_referencers)
	{
		Referencer->EnumerateLinks(LinkFunction);
	}
}