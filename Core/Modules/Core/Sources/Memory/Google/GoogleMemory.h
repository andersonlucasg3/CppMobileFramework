#pragma once

#include "Memory/Memory.h"

class CGoogleMemory : public CMemory
{
public:
	CORE_API CGoogleMemory() = default;
	CORE_API ~CGoogleMemory() override = default;

	CORE_API virtual void* Alloc(SizeT InSize) const override;
	CORE_API virtual void Free(void* InPtr) const override;
	CORE_API virtual void* Copy(const void* From, void* To, SizeT Size) const override;
	CORE_API virtual void* Set(void* Ptr, int Val, SizeT Size) const override;
	CORE_API virtual bool Equal(const void* Ptr1, SizeT Ptr1Size, const void* Ptr2, SizeT Ptr2Size) const override;
};

typedef CGoogleMemory CPlatformMemory;