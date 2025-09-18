#include "Thread.h"

#include "Preprocessors.h"

#include "Object/Collector/CollectorScope.h"
#include "ScopeLock.h"

#include COMPILE_PLATFORM_GROUP_HEADER(Thread.h)

static thread_local bool GIsMainThread = true;
static CPlatformThreadObjectPtr GMainThread;
// this will be overriden by each thread when started
static thread_local CThreadWeakObjectPtr GCurrentThread = GMainThread; // main thread

void CThread::Start(const TFunction<void(const CThreadWeakObjectPtr&)>& ThreadFunc)
{
    SScopeLock Lock(_isRunningSection);
    _bIsRunning = true;

    StartInternal([ThreadFunc](const CThreadWeakObjectPtr& Thread)
    {
        SCollectorScope ThreadScope;
        ThreadFunc(Thread);
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

TObjectPtr<CThread> CThread::Create()
{
    return new CPlatformThread;
}

CThread* CThread::Current()
{
    return GCurrentThread.Get();
}

bool CThread::IsMainThread()
{
    return GIsMainThread;
}

void CThread::ThreadStart(const CThreadWeakObjectPtr& Thread)
{
    GIsMainThread = false;
    GCurrentThread = Thread;
}

void CThread::ThreadEnd()
{
    GCurrentThread = nullptr;
}