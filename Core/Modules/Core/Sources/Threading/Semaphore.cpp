#include "Semaphore.h"

#include "ScopeLock.h"
#include <chrono>
#include <condition_variable>

SSemaphore::SSemaphore(int32_t Count) :
    Count(Count)
{
    //
}

SSemaphore::~SSemaphore()
{
    //
}

void SSemaphore::NotifyOne()
{
    std::unique_lock<std::mutex> Lock(Mutex);
    ++Count;
    Condition.notify_one();
}

void SSemaphore::Wait(Int64 InTimeoutMillis)
{
    std::unique_lock<std::mutex> Lock(Mutex);
    bool bSuccess = true;
    if (InTimeoutMillis > 0)
    {
        bSuccess = Condition.wait_for(Lock, std::chrono::milliseconds(InTimeoutMillis), [this] { return Count > 0; });
    }
    else
    {
        Condition.wait(Lock, [this] { return Count > 0; });
    }
    if (bSuccess)
    {
        --Count;
    }
}