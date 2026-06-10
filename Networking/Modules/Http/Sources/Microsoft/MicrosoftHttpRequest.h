#pragma once

#include "HttpRequest.h"

#include "HttpRequestError.h"

class CMicrosoftHttpRequest : public CHttpRequest
{
	using Super = CHttpRequest;

    HTTP_API CHttpRequestError GetError(CHttpRequestError::EHttpRequestError InError);

public:
	HTTP_API CMicrosoftHttpRequest() = default;
	HTTP_API ~CMicrosoftHttpRequest() override = default;

	HTTP_API void Process() override;
};

typedef CMicrosoftHttpRequest CPlatformHttpRequest;