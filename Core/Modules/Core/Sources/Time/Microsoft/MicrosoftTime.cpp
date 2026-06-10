#include "MicrosoftTime.h"

#include <Windows.h>

Double CWindowsTime::GetTimeInSeconds() const
{
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);

	LARGE_INTEGER Counter;
	QueryPerformanceCounter(&Counter);

	return static_cast<Double>(Counter.QuadPart) / static_cast<Double>(Frequency.QuadPart);
}