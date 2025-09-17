#include "AppleHttpRequestManager.h"

#include "HttpRequest.h"
#include "AppleHttpRequest.h"

#include "OperationQueue.h"
#include "String/Apple/AppleStringConvertion.h"

#include "NSSharedPtr.hpp"

#include "NSData.h"

#include "NSError.h"
#include "URLSession/NSURLSession.h"
#include "URLSession/NSURLSessionDataTask.h"
#include "URLSession/NSURLSessionConfiguration.h"

CAppleHttpRequestManager GAppleHttpRequestManager;

void CAppleHttpRequestManager::URLSessionDidBecomeInvalidWithError(URLSession*, Error* error)
{
    CHttpRequestError RequestError;

    RequestError.Error = CHttpRequestError::PlatformError;
    RequestError.ErrorCode = error->code();
    RequestError.Message = NSStringToCString(error->debugDescription());

    // SendErrorCallback(RequestError);
}

void CAppleHttpRequestManager::URLSessionTaskDidCompleteWithError(URLSession*, URLSessionTask* task, Error* error)
{
    if (CAppleHttpRequestObjectPtr* RequestPtr = _requestsMap.Find(task->hash()))
    {
        CAppleHttpRequest* Request = *RequestPtr;

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

void CAppleHttpRequestManager::URLSessionDataTaskDidReceiveData(URLSession*, URLSessionDataTask* dataTask, Data* data)
{
    if (CAppleHttpRequestObjectPtr* RequestPtr = _requestsMap.Find(dataTask->hash()))
    {
        CAppleHttpRequest* Request = *RequestPtr;

        Request->_response.AppendResponse(TArray<UInt8>(reinterpret_cast<UInt8*>(data->mutableBytes()), data->length()));
    }
}

void CAppleHttpRequestManager::URLSessionDataTaskDidReceiveResponse(URLSession*, URLSessionDataTask*, URLResponse*, const URLSessionDataTaskDidReceiveResponseCompletionHandler& completionHandler)
{
    completionHandler(URLSessionResponseAllow);
}

void CAppleHttpRequestManager::URLSessionTaskDidReceiveInformationalResponse(URLSession*, URLSessionTask*, HTTPURLResponse*)
{
    
}

CAppleHttpRequestManager::CAppleHttpRequestManager()
:   Super()
{
    OperationQueue* DelegateQueue = OperationQueue::alloc()->init(); // maybe I don't need this
    _urlSession = TransferPtr(URLSession::session(URLSessionConfiguration::defaultSessionConfiguration(), this, DelegateQueue));
    // TODO: _urlSession->setDelegate() here
}

CHttpRequest* CAppleHttpRequestManager::CreateRequest()
{
    CAppleHttpRequest* Request = new CAppleHttpRequest();
    Request->SetURLSession(_urlSession);
    return Request;
}

void CAppleHttpRequestManager::AddRequest(URLSessionTask* InTask, CAppleHttpRequest* InRequest)
{
    _requestsMap.Add(InTask->hash(), InRequest);
}