#include "MacHttpRequestManager.h"

#include "HttpRequest.h"
#include "Mac/MacHttpRequest.h"

#include "OperationQueue.h"
#include "String/Apple/AppleStringConvertion.h"

#include "NSSharedPtr.hpp"

#include "NSData.h"

#include "NSError.h"
#include "URLSession/NSURLSession.h"
#include "URLSession/NSURLSessionDataTask.h"
#include "URLSession/NSURLSessionConfiguration.h"

CMacHttpRequestManager GMacHttpRequestManager;

void CMacHttpRequestManager::URLSessionDidBecomeInvalidWithError(URLSession*, Error* error)
{
    CHttpRequestError RequestError;

    RequestError.Error = CHttpRequestError::PlatformError;
    RequestError.ErrorCode = error->code();
    RequestError.Message = NSStringToCString(error->debugDescription());

    // SendErrorCallback(RequestError);
}

void CMacHttpRequestManager::URLSessionTaskDidCompleteWithError(URLSession*, URLSessionTask* task, Error* error)
{
    if (CMacHttpRequestObjectPtr* RequestPtr = _requestsMap.Find(task->hash()))
    {
        CMacHttpRequest* Request = *RequestPtr;

        if (error != nullptr)
        {
            CHttpRequestError RequestError;
            RequestError.Error = CHttpRequestError::PlatformError;
            RequestError.ErrorCode = error->code();
            RequestError.Message = NSStringToCString(error->debugDescription());

            Request->SendErrorCallback(RequestError);

            return;
        }

        Request->_response.FinishResponse();

        Request->SendSuccessCallback();
    }
}

void CMacHttpRequestManager::URLSessionDataTaskDidReceiveData(URLSession*, URLSessionDataTask* dataTask, Data* data)
{
    if (CMacHttpRequestObjectPtr* RequestPtr = _requestsMap.Find(dataTask->hash()))
    {
        CMacHttpRequest* Request = *RequestPtr;

        Request->_response.AppendResponse(TArray<UInt8>(reinterpret_cast<UInt8*>(data->mutableBytes()), data->length()));
    }
}

void CMacHttpRequestManager::URLSessionDataTaskDidReceiveResponse(URLSession*, URLSessionDataTask*, URLResponse*, const URLSessionDataTaskDidReceiveResponseCompletionHandler& completionHandler)
{
    completionHandler(URLSessionResponseAllow);
}

void CMacHttpRequestManager::URLSessionTaskDidReceiveInformationalResponse(URLSession*, URLSessionTask*, HTTPURLResponse*)
{
    
}

CMacHttpRequestManager::CMacHttpRequestManager()
:   Super()
{
    OperationQueue* DelegateQueue = OperationQueue::alloc()->init(); // maybe I don't need this
    _urlSession = TransferPtr(URLSession::session(URLSessionConfiguration::defaultSessionConfiguration(), this, DelegateQueue));
    // TODO: _urlSession->setDelegate() here
}

CHttpRequest* CMacHttpRequestManager::CreateRequest()
{
    CMacHttpRequest* MacRequest = new CMacHttpRequest();
    MacRequest->SetURLSession(_urlSession);
    return MacRequest;
}

void CMacHttpRequestManager::AddRequest(URLSessionTask* InTask, CMacHttpRequest* InRequest)
{
    _requestsMap.Add(InTask->hash(), InRequest);
}