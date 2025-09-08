#include "Collector.h"

#include "Object/Object.h"
#include "Templates/Array.h"

void SCollector::WatchObject(CObject* Obj)
{
	_watching.Add(Obj);
}

void SCollector::UnWatchObject(CObject* Obj)
{
	_watching.Remove(Obj);
}

void SCollector::SetParent(SCollector *Parent)
{
	_parent = Parent;
}

SCollector* SCollector::Parent() const
{
	return _parent;
}

const TArray<CObject*>& SCollector::WatchedObjects() const
{
	return _watching;
}