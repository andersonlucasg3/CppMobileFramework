#include "MacHttpRequest.h"

#include "String/Apple/AppleStringConvertion.h"

#include "Mac/MacHttpRequestManager.h"

#include "URL/NSURL.h"
#include "URLSession/NSURLSession.h"
#include "URLSession/NSURLSessionDataTask.h"

using namespace NS;

void CMacHttpRequest::SetURLSession(const SharedPtr<URLSession> InURLSession)
{
    _urlSession = InURLSession;
}

void CMacHttpRequest::Process()
{
    URL* Url = URL::url(CStringToNSString(Endpoint()));

    URLSessionDataTask* DataTask = _urlSession->dataTask(Url);

    GMacHttpRequestManager.AddRequest(DataTask, this);

    DataTask->resume();
}