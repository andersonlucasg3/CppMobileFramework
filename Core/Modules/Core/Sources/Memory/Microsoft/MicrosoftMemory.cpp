#include "MicrosoftMemory.h"

#include <memory.h>
#include <cstdlib>

void* CWindowsMemory::Alloc(SizeT InSize) const
{
	return malloc(InSize);
}

void CWindowsMemory::Free(void* InPtr) const
{
	free(InPtr);
}

void* CWindowsMemory::Copy(const void* From, void* To, SizeT Size) const
{
	return memcpy(To, From, Size);
}

void* CWindowsMemory::Set(void* Ptr, int Val, SizeT Size) const
{
	return memset(Ptr, Val, Size);
}

bool CWindowsMemory::Equal(const void* Ptr1, SizeT Ptr1Size, const void* Ptr2, SizeT Ptr2Size) const
{
	if (Ptr1Size != Ptr2Size) return false;

	return memcmp(Ptr1, Ptr2, Ptr1Size) == 0; // equal
}