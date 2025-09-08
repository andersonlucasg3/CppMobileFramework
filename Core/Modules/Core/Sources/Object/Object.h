#pragma once

#include "Class.h"

#include "Templates/Array.h"

#include "Threading/CriticalSection.h"

#include "Properties/Property.h"
#include "Properties/ArrayProperty.h"

DECLARE_CLASS_HEADER(Object);

class CObject
{	
public:
	CORE_API CObject();
	CORE_API virtual ~CObject();

	bool IsRooted() const;
	void MakeRooted();
	void MakeUnrooted();

	bool IsQueuedForDestruction() const;

	template<typename TOther>
	bool IsA()
	{
		return dynamic_cast<TOther*>(this) != nullptr;
	}

private:
	TArray<Objects::Properties::CProperty*> _properties;

	SCriticalSection _criticalSection;
	bool _bIsRooted = false;

	/** do not call directly, use SetQueuedForDestruction() */
	bool _bIsQueuedForDestruction = false;

	void SetQueuedForDestruction(bool bIsQueuedForDestruction);

	friend class CObjectCollector;

	friend class Objects::Properties::CProperty;
};