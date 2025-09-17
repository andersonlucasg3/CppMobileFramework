#include "HttpRequest.h"

#include "Defines/Asserts.h"
#include "Defines/Preprocessors.h"

#include "HttpRequestError.h"

#include COMPILE_PLATFORM_GROUP_HEADER(HttpRequest.h)

const char* ToString(EHttpRequestMethod InMethod)
{
    switch (InMethod)
    {
        case EHttpRequestMethod::Get: return "GET";
        case EHttpRequestMethod::Post: return "POST";
        default:
            assertm(false, "Implement new cases");
            return "";
    }
}

void CHttpRequest::SendErrorCallback(const CHttpRequestError& InError)
{
    _callbacks->HttpRequestFailedWithError(this, InError);
}

void CHttpRequest::SendSuccessCallback()
{
    _callbacks->HttpRequestSucceeded(this, _response);
}

const CString& CHttpRequest::Endpoint() const
{
    return _endpoint;
}

EHttpRequestMethod CHttpRequest::Method() const
{
    return _method;
}

const TMap<CString, CString>& CHttpRequest::Headers() const
{
    return _headers;
}

const CHttpRequestCallbacksPtr& CHttpRequest::Callbacks() const
{
    return _callbacks;
}

CHttpRequest& CHttpRequest::SetEndpoint(const CString& InEndpoint)
{
    _endpoint = InEndpoint;

    return *this;
}

CHttpRequest& CHttpRequest::SetMethod(EHttpRequestMethod InMethod)
{
    _method = InMethod;

    return *this;
}

CHttpRequest& CHttpRequest::SetHeader(const CString& InKey, const CString& InValue)
{
    _headers.Set(InKey, InValue);

    return *this;
}

CHttpRequest& CHttpRequest::SetCallbacks(const CHttpRequestCallbacksPtr& InCallbacks)
{
    _callbacks = InCallbacks;

    return *this;
}