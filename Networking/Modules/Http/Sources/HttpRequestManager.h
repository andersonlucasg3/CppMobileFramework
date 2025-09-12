#pragma once

#include "Object/Object.h"

#include "Threading/Thread.h"
#include "Threading/CriticalSection.h"

#include "HttpRequest.h"

class CHttpRequestManager : public CObject
{
public:
    HTTP_API CHttpRequestManager();
    HTTP_API virtual ~CHttpRequestManager();

    HTTP_API void StartRequestWorker();
    HTTP_API void StopRequestWorker();

    HTTP_API virtual CHttpRequest* CreateRequest() = 0;

    HTTP_API void AddRequest(CHttpRequest* InRequest);

private:
    SCriticalSection _httpThreadCS;
    CThreadObjectPtr _httpThread;

    SCriticalSection _pendingRequestsCS;
    
    TQueue<CHttpRequestObjectPtr> _pendingRequests;

    void ThreadWorker(const CThreadWeakObjectPtr& Thread);
};

HTTP_API extern CHttpRequestManager& GHttpRequestManager;