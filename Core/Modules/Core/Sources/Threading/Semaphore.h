#pragma once

#include "Defines/Types.h"

#include <mutex>

using namespace Core::Types;

struct SSemaphore
{
    CORE_API SSemaphore(int32_t Count = 0);
    CORE_API ~SSemaphore();

    CORE_API void NotifyOne();
    CORE_API void Wait(Int64 InTimeoutMillis = -1);

private:
    std::mutex Mutex;
    std::condition_variable Condition;
    int32_t Count;
};