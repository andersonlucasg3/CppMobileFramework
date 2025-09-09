#pragma once

#include "Class.h"

#include "Templates/Array.h"

#include "Threading/CriticalSection.h"

#include "Properties/Property.h"
#include "Properties/ArrayProperty.h"

#include "Object/Collector/Referencer.h"

DECLARE_CLASS_HEADER(Object);

class CObject
{	
public:
	CORE_API CObject();
	CORE_API virtual ~CObject() = default;

	CORE_API bool IsRooted() const;
	CORE_API void MakeRooted();
	CORE_API void MakeUnrooted();

	CORE_API bool IsQueuedForDestruction() const;

	template<typename TOther>
	bool IsA()
	{
		return dynamic_cast<TOther*>(this) != nullptr;
	}

private:
	TArray<CReferencer*> _referencers;

	SCriticalSection _criticalSection;
	bool _bIsRooted = false;

	/** do not call directly, use SetQueuedForDestruction() */
	bool _bIsQueuedForDestruction = false;

	void SetQueuedForDestruction(bool bIsQueuedForDestruction);

	friend class CReferencer;
	friend class CObjectCollector;
};