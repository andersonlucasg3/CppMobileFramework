#include "HttpRequestTests.h"

#include "Threading/Semaphore.h"

#include "TestSuit.h"

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpRequestError.h"
#include "HttpRequestManager.h"

REGISTER_TEST_CLASS(HttpRequestTests);

class CRequestCallbacks : public CHttpRequestCallbacks
{
    SSemaphore* _semaphore;

public:
    CRequestCallbacks(SSemaphore* InSemaphore) : _semaphore(InSemaphore) 
    { 
        //
    }

    ~CRequestCallbacks() override = default;

    void HttpRequestFailedWithError(CHttpRequest*, const CHttpRequestError& InError) override
    {
        GLogger.Log("Request Error: {}, Message: {}", ToString(InError.Error), *InError.Message);

        _semaphore->NotifyOne();
    }

    
    void HttpRequestSucceeded(CHttpRequest*, const CHttpResponse& InResponse) override
    {
        GLogger.Log("Status: {}, Response: {}", InResponse.Status(), *InResponse.ResponseString());
        
        _semaphore->NotifyOne();
    }
};

void CHttpRequestTests::TestCase()
{
    GHttpRequestManager.StartRequestWorker();

    SSemaphore RequestSemaphore;

    CHttpRequest* Request = GHttpRequestManager.CreateRequest();
    
    Request->SetEndpoint("https://jsonplaceholder.typicode.com/todos/1")
        .SetMethod(EHttpRequestMethod::Get)
        .SetCallbacks(MakeShared<CRequestCallbacks>(&RequestSemaphore));

    GHttpRequestManager.AddRequest(Request);

    RequestSemaphore.Wait();

    GHttpRequestManager.StopRequestWorker();
}