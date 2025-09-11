#include "HttpRequestManager.h"

#include "Defines/Preprocessors.h"

#include "HttpRequest.h"
#include "Threading/ScopeLock.h"
#include "Threading/Thread.h"

#include COMPILE_PLATFORM_HEADER(HttpRequestManager.h)

static CPlatformHttpRequestManager GPlatformHttpRequestManager;
CHttpRequestManager& GHttpRequestManager = GPlatformHttpRequestManager;

void CHttpRequestManager::ThreadWorker(const CThreadWeakPtr& Thread)
{
    SScopeLock ScopeLock(_pendingRequestsCS);

    if (_pendingRequests.IsEmpty())
    {
        Thread->Sleep(100);

        return;
    }

    CHttpRequestPtr Request;
    while (_pendingRequests.Dequeue(Request))
    {
        Request->Process();
    }
}

CHttpRequestManager::CHttpRequestManager()
:   _httpThread(CThread::Create())
{
    MakeRooted(); // never gets destroyed
}

CHttpRequestManager::~CHttpRequestManager()
{
    StopRequestWorker();
}

void CHttpRequestManager::StartRequestWorker()
{
    // TODO: implement a SharedPtr with thread safety abstraction so we don't need this
    SScopeLock ScopeLock(_httpThreadCS);

    if (_httpThread->IsRunning()) return;

    _httpThread->Start([this](const CThreadWeakPtr& Thread)
    {
        ThreadWorker(Thread);
    });
}

void CHttpRequestManager::StopRequestWorker()
{
    // TODO: implement a SharedPtr with thread safety abstraction so we don't need this
    SScopeLock ScopeLock(_httpThreadCS); 

    if (!_httpThread || !_httpThread->IsRunning()) return;

    _httpThread->Exit();
    _httpThread->Join();

    _httpThread = nullptr;
}

void CHttpRequestManager::AddRequest(const CHttpRequestPtr& InRequest)
{
    SScopeLock ScopeLock(_pendingRequestsCS);
    
    _pendingRequests.Enqueue(InRequest);

    StartRequestWorker();
}