#include "MicrosoftHttpRequest.h"

#include "HttpRequestError.h"

#include "String/String.h"

#include <Windows.h>
#include <errhandlingapi.h>
#include <minwinbase.h>
#include <winbase.h>
#include <winhttp.h>
#include <winnt.h>

CHttpRequestError CWindowsHttpRequest::GetError(CHttpRequestError::EHttpRequestError InError)
{
    CHttpRequestError RequestError;
    RequestError.Error = InError;

    DWORD ErrorCode = GetLastError();
    LPSTR ErrorMessage = NULL;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        ErrorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        ErrorMessage,
        0, NULL
    );

    RequestError.Message = ErrorMessage;

    LocalFree(ErrorMessage);

    return RequestError;
}

void ReleaseHandles(const TArray<HINTERNET>& InHandles)
{
    for (HINTERNET Handle : InHandles)
    {
        WinHttpCloseHandle(Handle);
    }
}

void CWindowsHttpRequest::Process()
{
    HINTERNET Session = WinHttpOpen(L"WinHTTP Request/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (!Session)
    {
        SendErrorCallback(GetError(CHttpRequestError::PlatformApiError));

        return;
    }

    CWString WEndpoint = Endpoint().WStr();
    HINTERNET Connect = WinHttpConnect(Session, WEndpoint.Raw(), INTERNET_DEFAULT_HTTP_PORT, 0);

    if (!Connect)
    {
        ReleaseHandles({ Session });

        SendErrorCallback(GetError(CHttpRequestError::PlatformApiError));

        return;
    }

    CWString WMethod = CString(ToString(Method())).WStr();
    HINTERNET Request = WinHttpOpenRequest(Connect, WMethod.Raw(), L"/", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

    if (!Request)
    {
        ReleaseHandles({ Connect, Session });

        SendErrorCallback(GetError(CHttpRequestError::PlatformApiError));

        return;
    }

    CString HeadersString;
    CWString WHeadersString;
    LPWSTR Headers = WINHTTP_NO_ADDITIONAL_HEADERS;
    if (Super::Headers().Num() > 0)
    {
        // fill headers string
        HeadersString = "";
        WHeadersString = HeadersString.WStr();
        Headers = WHeadersString.Raw();
    }

    bool bResult = WinHttpSendRequest(Request, Headers, HeadersString.Len(), WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

    if (!bResult)
    {
        ReleaseHandles({ Request, Connect, Session });

        SendErrorCallback(GetError(CHttpRequestError::PlatformApiError));

        return;
    }

    bResult = WinHttpReceiveResponse(Request, NULL);

    if (!bResult)
    {
        ReleaseHandles({ Request, Connect, Session });

        SendErrorCallback(GetError(CHttpRequestError::NoResponseError));

        return;
    }

    DWORD Size = 0;
    DWORD Downloaded = 0;
    CString ResponseString;
    while (WinHttpQueryDataAvailable(Request, &Size))
    {
        LPSTR Buffer = new char[Size + 1];
        ZeroMemory(Buffer, Size + 1);

        if (!WinHttpReadData(Request, Buffer, Size, &Downloaded))
        {
            delete[] Buffer;

            ReleaseHandles({ Request, Connect, Session });

            SendErrorCallback(GetError(CHttpRequestError::NoResponseError));

            return;
        }
        
        // TODO: implement a string view struct that receives the start and end in a buffer
        // concatenating should not copy the buffer to a string before adding to the current string
        // should read directly from the buffer without needing an extra copy
        ResponseString += CString(Buffer, Downloaded);

        delete[] Buffer;
    }

    ReleaseHandles({ Request, Connect, Session });

    SendSuccessCallback(ResponseString);
}