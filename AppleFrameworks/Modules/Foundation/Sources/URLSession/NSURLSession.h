#pragma once

#include "NSObject.hpp"
#include "NSDefines.h"

namespace NS
{
    _NS_ENUM(Integer, URLSessionDelayedRequestDisposition)
    {
        URLSessionDelayedRequestContinueLoading = 0,
        URLSessionDelayedRequestUseNewRequest = 1,
        URLSessionDelayedRequestCancel = 2,
    };

    _NS_ENUM(Integer, URLSessionResponseDisposition) 
    {
        URLSessionResponseCancel = 0,
        URLSessionResponseAllow = 1,
        URLSessionResponseBecomeDownload = 2,
        URLSessionResponseBecomeStream = 3,
    };

    class URLSession : public Copying<URLSession>
    {
    public:
        FOUNDATION_API static URLSession* sharedSession();

        FOUNDATION_API static URLSession* session(class URLSessionConfiguration* configuration);
        FOUNDATION_API static URLSession* session(class URLSessionConfiguration* configuration, class URLSessionDelegate* delegate, class OperationQueue* delegateQueue);

        FOUNDATION_API class URLSessionDataTask* dataTask(class URL* url);

    private:
        static void* CreateDelegate(class URLSessionDelegate* delegate);
    };
}