#include "GoogleMemory.h"

#include "Defines/Types.h"

#include <memory.h>
#include <cstdlib>

void* CGoogleMemory::Alloc(SizeT InSize) const
{
	return malloc(InSize);
}

void CGoogleMemory::Free(void* InPtr) const
{
	free(InPtr);
}

void* CGoogleMemory::Copy(const void* From, void* To, SizeT Size) const
{
	return memcpy(To, From, Size);
}

void* CGoogleMemory::Set(void* Ptr, int Val, SizeT Size) const
{
	return memset(Ptr, Val, Size);
}

bool CGoogleMemory::Equal(const void* Ptr1, SizeT Ptr1Size, const void* Ptr2, SizeT Ptr2Size) const
{
	if (Ptr1Size != Ptr2Size) return false;

	return memcmp(Ptr1, Ptr2, Ptr1Size) == 0; // equal
}