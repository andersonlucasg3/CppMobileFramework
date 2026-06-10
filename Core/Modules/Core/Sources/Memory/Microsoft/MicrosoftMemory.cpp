#include "MicrosoftMemory.h"

#include <memory.h>
#include <cstdlib>

void* CMicrosoftMemory::Alloc(SizeT InSize) const
{
	return malloc(InSize);
}

void CMicrosoftMemory::Free(void* InPtr) const
{
	free(InPtr);
}

void* CMicrosoftMemory::Copy(const void* From, void* To, SizeT Size) const
{
	return memcpy(To, From, Size);
}

void* CMicrosoftMemory::Set(void* Ptr, int Val, SizeT Size) const
{
	return memset(Ptr, Val, Size);
}

bool CMicrosoftMemory::Equal(const void* Ptr1, SizeT Ptr1Size, const void* Ptr2, SizeT Ptr2Size) const
{
	if (Ptr1Size != Ptr2Size) return false;

	return memcmp(Ptr1, Ptr2, Ptr1Size) == 0; // equal
}