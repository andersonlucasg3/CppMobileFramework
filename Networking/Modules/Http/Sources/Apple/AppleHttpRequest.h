#pragma once

#include "HttpRequest.h"

#include "Object/ClassMacros.h"

#include "NSSharedPtr.hpp"

#include "URLSession/NSURLSessionDataDelegate.h"

FORWARD_DECLARE_OBJECT(AppleHttpRequest);

using namespace NS;

class CAppleHttpRequest : public CHttpRequest, protected URLSessionDataDelegate
{
    using Super = CHttpRequest;

    SharedPtr<URLSession> _urlSession;

public:
    HTTP_API CAppleHttpRequest() = default;
    HTTP_API ~CAppleHttpRequest() override = default;

    HTTP_API void SetURLSession(const SharedPtr<class URLSession> InURLSession);

    HTTP_API void Process() override;

    friend class CAppleHttpRequestManager;
};

typedef CAppleHttpRequest CPlatformHttpRequest;