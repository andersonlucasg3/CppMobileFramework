#include "GoogleTime.h"

#include <ctime>

Double CAndroidTime::GetTimeInSeconds() const
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // or CLOCK_REALTIME
    return ts.tv_sec + ts.tv_nsec / 1e9;
}