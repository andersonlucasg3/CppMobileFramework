#include "AppleHttpRequest.h"

#include "String/Apple/AppleStringConvertion.h"

#include "AppleHttpRequestManager.h"

#include "URL/NSURL.h"
#include "URLSession/NSURLSession.h"
#include "URLSession/NSURLSessionDataTask.h"

using namespace NS;

void CAppleHttpRequest::SetURLSession(const SharedPtr<URLSession> InURLSession)
{
    _urlSession = InURLSession;
}

void CAppleHttpRequest::Process()
{
    URL* Url = URL::url(CStringToNSString(Endpoint()));

    URLSessionDataTask* DataTask = _urlSession->dataTask(Url);

    GAppleHttpRequestManager.AddRequest(DataTask, this);

    DataTask->resume();
}