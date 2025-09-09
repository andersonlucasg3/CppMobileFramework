#include "MacThread.h"

#include "SmartPointer/MakeAndCasts.h"

#include "Threading/Thread.h"

#include "NSAutoreleasePool.h"

#include <chrono>
#include <thread>

void CMacThread::SetName(const CString &Name)
{
    ThreadName = Name;
}

const CString& CMacThread::Name() const
{
    return ThreadName;
}

void CMacThread::Start(const TFunction<void(CThread*)>& ThreadFunc)
{
    Super::Start(ThreadFunc);

    if (!Thread)
    {
        Thread = MakeShared<std::thread>([this, ThreadFunc]
        {
            pthread_setname_np(*ThreadName.SubString(0, 15));
            while(IsRunning())
            {
                NS::AutoreleasePool* Pool = NS::AutoreleasePool::alloc()->init();

                ThreadFunc(this);

                Pool->drain();
            }
        });
    }
}

void CMacThread::Join()
{
    if (Thread && Thread->joinable())
    {
        Thread->join();
    }
}

void CMacThread::Sleep(UInt64 InMilliseconds) const
{
    std::this_thread::sleep_for(std::chrono::milliseconds(InMilliseconds));
}