#include "GoogleThread.h"

#include <chrono>

void CGoogleThread::SetName(const CString& Name)
{
    _threadName = Name;
}

const CString& CGoogleThread::Name() const
{
    return _threadName;
}

void CGoogleThread::Join()
{
    if (Thread && Thread->joinable())
    {
        Thread->join();
    }
}

void CGoogleThread::Sleep(UInt64 InTimeMilliseconds) const
{
    std::this_thread::sleep_for(std::chrono::milliseconds(InTimeMilliseconds));
}

void CGoogleThread::StartInternal(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc)
{
    if (!Thread)
    {
        Thread = MakeShared<std::thread>([this, ThreadFunc]
        {
            while (IsRunning())
            {
                ThreadFunc(this);
            }
        });
    }
}

CGoogleThreadPtr CGoogleThread::Create()
{
    return MakeShared<CGoogleThread>();
}