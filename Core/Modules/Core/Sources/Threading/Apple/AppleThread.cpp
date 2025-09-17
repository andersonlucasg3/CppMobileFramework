#include "AppleThread.h"

#include "SmartPointer/MakeAndCasts.h"

#include "Threading/Thread.h"

#include "NSAutoreleasePool.h"

#include <chrono>
#include <thread>

void CAppleThread::SetName(const CString &Name)
{
    _threadName = Name;
}

const CString& CAppleThread::Name() const
{
    return _threadName;
}

void CAppleThread::Join()
{
    if (_thread && _thread->joinable())
    {
        _thread->join();
    }
}

void CAppleThread::Sleep(UInt64 InMilliseconds) const
{
    std::this_thread::sleep_for(std::chrono::milliseconds(InMilliseconds));
}

void CAppleThread::StartInternal(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc)
{
    if (!_thread)
    {
        _thread = MakeShared<std::thread>([this, ThreadFunc]
        {
            ThreadStart(this);
            {
                pthread_setname_np(*_threadName.SubString(0, 15));

                while(IsRunning())
                {
                    NS::AutoreleasePool* Pool = NS::AutoreleasePool::alloc()->init();

                    ThreadFunc(this);

                    Pool->drain();
                }
            }
            ThreadEnd();
        });
    }
}