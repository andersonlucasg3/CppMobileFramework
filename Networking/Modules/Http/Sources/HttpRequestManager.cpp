#include "HttpRequestManager.h"

#include "Preprocessors.h"

#include "Threading/ScopeLock.h"
#include "Threading/Thread.h"

#include "Object/Collector/CollectorScope.h"

#include "HttpRequest.h"

#include COMPILE_PLATFORM_GROUP_HEADER(HttpRequestManager.h)

static CPlatformHttpRequestManager GPlatformHttpRequestManager;
CHttpRequestManager& GHttpRequestManager = GPlatformHttpRequestManager;

void CHttpRequestManager::ThreadWorker(const CThreadWeakObjectPtr& Thread)
{
    if (!Thread.IsValid()) return;

    if (_pendingRequests.IsEmpty())
    {
        _processQueueSemaphore.Wait();
    }

    CHttpRequestObjectPtr Request = DequeueRequest();
    while (Request.IsValid())
    {
        Request->Process();

        Request = DequeueRequest();
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
    if (_httpThread->IsRunning()) 
    {
        _processQueueSemaphore.NotifyOne();

        return;
    }

    _httpThread->Start([this](const CThreadWeakObjectPtr& Thread)
    {
        SCollectorScope Scope;
        ThreadWorker(Thread);
    });
}

void CHttpRequestManager::StopRequestWorker()
{
    if (!_httpThread || !_httpThread->IsRunning()) return;

    _httpThread->Exit();
    _httpThread->Join();

    _httpThread = nullptr;
}

void CHttpRequestManager::AddRequest(CHttpRequest* InRequest)
{
    SScopeLock ScopeLock(_pendingRequestsCS);
    
    _pendingRequests.Enqueue(InRequest);

    StartRequestWorker();
}

CHttpRequestObjectPtr CHttpRequestManager::DequeueRequest()
{
    SScopeLock ScopeLock(_pendingRequestsCS);

    CHttpRequestObjectPtr Request;
    _pendingRequests.Dequeue(Request);
    return Request;
}