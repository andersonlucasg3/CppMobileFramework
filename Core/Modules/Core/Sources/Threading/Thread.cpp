#include "Thread.h"

#include "Defines/Preprocessors.h"

#include "ScopeLock.h"

#include COMPILE_PLATFORM_HEADER(Thread.h)

static thread_local bool GIsMainThread = true;
static thread_local TSharedPtr<CPlatformThread> GMainThread;
// this will be overriden by each thread when started
static thread_local TWeakPtr<CThread> GCurrentThread = GMainThread; // main thread

void CThread::Start(const TFunction<void(const TWeakPtr<CThread>&)>& ThreadFunc)
{
    SScopeLock Lock(_isRunningSection);
    _bIsRunning = true;

    StartInternal([TFunc = ThreadFunc](const TWeakPtr<CThread>& WeakThread)
    {
        GIsMainThread = false;
        GCurrentThread = WeakThread;
        TFunc(WeakThread);
        GCurrentThread = nullptr;
    });
}

void CThread::Exit()
{
    SScopeLock Lock(_isRunningSection);
    _bIsRunning = false;
}

bool CThread::IsRunning() const
{
    SScopeLock Lock(_isRunningSection);
    return _bIsRunning;
}

CThreadPtr CThread::Create()
{
    return MakeShared<CPlatformThread>();
}

CThread& CThread::Current()
{
    return *GCurrentThread;
}

bool CThread::IsMainThread()
{
    return GIsMainThread;
}