#pragma once

#if PLATFORM_WINDOWS

#include "Memory/Memory.h"

class CWindowsMemory : public CMemory
{
public:
	CWindowsMemory() = default;
	~CWindowsMemory() override = default;

	CORE_API void* Alloc(SizeT InSize) const override;
	CORE_API void Free(void* InPtr) const override;
	CORE_API void* Copy(const void* From, void* To, SizeT Size) const override;
	CORE_API void* Set(void* Ptr, int Val, SizeT Size) const override;
	CORE_API bool Equal(const void* Ptr1, SizeT Ptr1Size, const void* Ptr2, SizeT Ptr2Size) const override;
};

typedef CWindowsMemory CPlatformMemory;

#endif // PLATFORM_WINDOWS