#pragma once

#include "Templates/Hashable.h"

struct SObjectCollectedListenerHandle : CHashable
{
	CORE_API SObjectCollectedListenerHandle() = default;
	CORE_API ~SObjectCollectedListenerHandle() override = default;

	CORE_API bool IsValid() const;

	CORE_API SizeT Hash() const override;

	CORE_API bool operator==(const SObjectCollectedListenerHandle& Other) const;

private:
	Int64 _handle = -1; // invalid

	CORE_API SObjectCollectedListenerHandle(Int64 Handle);

	CORE_API void Invalidate();

	CORE_API operator Int64() const;

	friend class CObjectCollector;
};