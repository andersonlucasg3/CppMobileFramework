#pragma once

#include "Templates/Map.h"

#include "HttpRequest.h"
#include "HttpRequestManager.h"
#include "Mac/MacHttpRequest.h"

#include "NSSharedPtr.hpp"

#include "URLSession/NSURLSession.h"
#include "URLSession/NSURLSessionDataDelegate.h"

using namespace NS;

class CMacHttpRequestManager : public CHttpRequestManager, public URLSessionDataDelegate
{
    using Super = CHttpRequestManager;

    SharedPtr<URLSession> _urlSession;

    TMap<Integer, CMacHttpRequestObjectPtr> _requestsMap;

protected:
    HTTP_API void URLSessionDidBecomeInvalidWithError(URLSession* Session, Error* error) override;
    HTTP_API void URLSessionTaskDidCompleteWithError(URLSession* session, URLSessionTask* task, Error* error) override;
    HTTP_API void URLSessionDataTaskDidReceiveData(URLSession* session, URLSessionDataTask* dataTask, Data* data) override;
    HTTP_API void URLSessionDataTaskDidReceiveResponse(URLSession* session, URLSessionDataTask* dataTask, URLResponse* response, const URLSessionDataTaskDidReceiveResponseCompletionHandler& completionHandler) override;
    HTTP_API void URLSessionTaskDidReceiveInformationalResponse(URLSession* session, URLSessionTask* task, HTTPURLResponse* response) override;

public:
    HTTP_API CMacHttpRequestManager();
    HTTP_API ~CMacHttpRequestManager() override = default;

    HTTP_API CHttpRequest* CreateRequest() override;

    HTTP_API void AddRequest(URLSessionTask* InTask, CMacHttpRequest* InRequest);
};

HTTP_API extern CMacHttpRequestManager GMacHttpRequestManager;

typedef CMacHttpRequestManager CPlatformHttpRequestManager;