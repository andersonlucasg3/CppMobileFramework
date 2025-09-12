#pragma once

#include "Defines/Types.h"

#include "Templates/Map.h"

#include "SmartPointer/SharedFromThis.h"

#include "Object/Object.h"
#include "Object/ClassMacros.h"

#include "String/String.h"

#include "HttpResponse.h"
#include "HttpRequestError.h"

FORWARD_DECLARE_OBJECT(HttpRequest);
FORWARD_DECLARE_SHARED(HttpRequestCallbacks);

enum class EHttpRequestMethod : UInt8
{
    Get,
    Post

    // if you add new values, update ToString implementation
};

HTTP_API const char* ToString(EHttpRequestMethod InMethod);

class CHttpRequestCallbacks
{
public:
    HTTP_API virtual ~CHttpRequestCallbacks() = default;

    HTTP_API virtual void HttpRequestFailedWithError(CHttpRequest* InRequest, const CHttpRequestError& InError) = 0;
    // TODO: instead implement a response object with status and additional response information
    HTTP_API virtual void HttpRequestSucceeded(CHttpRequest* InRequest, const CHttpResponse& InResponse) = 0;
};

class CHttpRequest : public CObject
{
    CString _endpoint;
    EHttpRequestMethod _method;
    TMap<CString, CString> _headers;

    CHttpRequestCallbacksPtr _callbacks;

protected:
    CHttpResponse _response;

    HTTP_API void SendErrorCallback(const CHttpRequestError& InError);
    HTTP_API void SendSuccessCallback();

public:
    HTTP_API CHttpRequest() = default;
    HTTP_API virtual ~CHttpRequest() = default;

    HTTP_API const CHttpResponse& Response() const;

    HTTP_API const CString& Endpoint() const;
    HTTP_API EHttpRequestMethod Method() const;
    HTTP_API const TMap<CString, CString>& Headers() const;
    HTTP_API const CHttpRequestCallbacksPtr& Callbacks() const;
    
    HTTP_API CHttpRequest& SetEndpoint(const CString& InEndpoint);
    HTTP_API CHttpRequest& SetMethod(EHttpRequestMethod InMethod);
    HTTP_API CHttpRequest& SetHeader(const CString& InKey, const CString& InValue);
    HTTP_API CHttpRequest& SetCallbacks(const CHttpRequestCallbacksPtr& InCallbacks);

    HTTP_API virtual void Process() = 0;
};