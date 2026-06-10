#pragma once

#include "HttpRequest.h"

#include "HttpRequestError.h"

class CWindowsHttpRequest : public CHttpRequest
{
	using Super = CHttpRequest;

    HTTP_API CHttpRequestError GetError(CHttpRequestError::EHttpRequestError InError);

public:
	HTTP_API CWindowsHttpRequest() = default;
	HTTP_API ~CWindowsHttpRequest() override = default;

	HTTP_API void Process() override;
};

typedef CWindowsHttpRequest CPlatformHttpRequest;