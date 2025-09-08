#include "ObjectCollector.h"

#include "Templates/Array.h"

#include "Collector.h"
#include "ObjectLink.h"

#include "Object/Object.h"
#include "Object/Properties/Property.h"
#include "Threading/ScopeLock.h"
#include <stdexcept>

using namespace Objects::Properties;

static CObjectCollector ObjectCollector;
CObjectCollector& GObjectCollector = ObjectCollector;

CObjectCollector::CObjectCollector()
:   _currentCollector(nullptr)
{
    
}

CObjectCollector::~CObjectCollector()
{
    // TODO: anything here?
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

CObjectLink* CObjectCollector::AddObjectLink(CObject* Obj, CProperty* Property)
{
	if (Obj == nullptr) return nullptr;
	
	SScopeLock Lock(_objectsCriticalSection);

	if (Obj->IsQueuedForDestruction())
	{
		Obj->SetQueuedForDestruction(false);
	}
	
	CObjectLink* Link = new CObjectLink(Property, Obj);
	
	TArray<CObjectLink*>* LinksArray = _objectLinksMap.Find(Obj);
	if (LinksArray == nullptr)
	{
		LinksArray = &_objectLinksMap.Add(Obj, TArray<CObjectLink*>());
	}

	LinksArray->Add(Link);
	
	return Link;
}

void CObjectCollector::RemoveObjectLink(CObjectLink* Link)
{
	if (Link == nullptr) return;

	SScopeLock Lock(_objectsCriticalSection);
	
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

bool CObjectCollector::HasLinks(CObject* Obj) const
{
	SScopeLock Lock(_objectsCriticalSection);

	if (TArray<CObjectLink*>* LinksArray = _objectLinksMap.Find(Obj))
	{
		return LinksArray->Num() > 0;
	}
	
	return false;
}

void CObjectCollector::WatchObject(CObject *Obj)
{
	SScopeLock Lock(_objectsCriticalSection);

	_globalObjects.Add(Obj);

	if (Obj->IsRooted()) return;
	
	if (SCollector* Collector = _currentCollector)
	{
		Collector->WatchObject(Obj);
	}
}

void CObjectCollector::UnWatchObject(CObject *Obj)
{
	SScopeLock Lock(_objectsCriticalSection);

	_globalObjects.Remove(Obj);

	if (Obj->IsRooted()) return;
	
	if (SCollector* Collector = _currentCollector)
	{
		Collector->UnWatchObject(Obj);
	}
}

void CObjectCollector::AddToRoot(CObject* Obj)
{
	SScopeLock Lock(_objectsCriticalSection);

	_rootedObjects.Add(Obj);

	if (_currentCollector != nullptr)
	{
		_currentCollector->UnWatchObject(Obj);
	}
}

void CObjectCollector::RemoveFromRoot(CObject* Obj)
{
	SScopeLock Lock(_objectsCriticalSection);

	_rootedObjects.Remove(Obj);

	if (_currentCollector != nullptr)
	{
		_currentCollector->WatchObject(Obj);
	}
}

SizeT CObjectCollector::AliveObjectCount() const
{
	SScopeLock Lock(_objectsCriticalSection);

	return _globalObjects.Num();
}

void CObjectCollector::CollectGarbage()
{
	SScopeLock ObjectsLock(_objectsCriticalSection);

	TArray<CObject*> Marked;

	for (CObject* Object : _rootedObjects)
	{
		RecursivelyMarkObjects(Object, Marked);
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
}

void CObjectCollector::SetQueuedForDestruction(CObject* Obj, bool bEnqueue)
{
	SScopeLock Lock(_destructionQueueCriticalSection);

	if (bEnqueue)
	{
		_destructionQueue.Add(Obj);
	}
	else
	{
		_destructionQueue.Remove(Obj);
	}
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
	for (CProperty* Property : Obj->_properties)
	{
		Property->ReleaseLinks();
	}

	// do some index logic to fast removal
	_globalObjects.Remove(Obj);
	TArray<CObjectLink*> Links;
	_objectLinksMap.RemoveAndCopyValue(Obj, Links);

	if (Links.Num() > 0) throw new std::runtime_error("should not have any live links");

	delete Obj;
}

void CObjectCollector::RecursivelyMarkObjects(CObject* InRootObject, TArray<CObject*>& RefMarked)
{
	static auto LinkFunction = [this, &RefMarked](CObjectLink* Link)
	{
		this->RecursivelyMarkObjects(Link->Object(), RefMarked);
	};

	RefMarked.Add(InRootObject);

	for (CProperty* Property : InRootObject->_properties)
	{
		Property->EnumerateLinks(LinkFunction);
	}
}