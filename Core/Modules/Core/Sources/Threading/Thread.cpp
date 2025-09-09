#include "Thread.h"

#include "Defines/Preprocessors.h"

#include "ScopeLock.h"

#include COMPILE_PLATFORM_HEADER(Thread.h)

bool CThread::IsRunning()
{
    SScopeLock Lock(_isRunningSection);
    return _bIsRunning;
}

void CThread::Start(const TFunction<void(CThread*)>&)
{
    SScopeLock Lock(_isRunningSection);
    _bIsRunning = true;
}

void CThread::Exit()
{
    SScopeLock Lock(_isRunningSection);
    _bIsRunning = false;
}

CThread* CThread::Create()
{
    return new CPlatformThread();
}