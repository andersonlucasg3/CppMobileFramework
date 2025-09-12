#include "NSURLSession.h"

#include "NSObject.hpp"
#include "NSPrivate.h"

#include "OperationQueue.h"

#include "URLSession/NSURLSessionConfiguration.h"
#include "URLSession/NSURLSessionDelegate.h"

namespace NS 
{
    URLSession* URLSession::sharedSession()
    {
        return Object::sendMessage<URLSession*>(_NS_PRIVATE_CLS(NSURLSession), _NS_PRIVATE_SEL(sharedSession));
    }

    URLSession* URLSession::session(URLSessionConfiguration* configuration)
    {
        return Object::sendMessage<URLSession*>(_NS_PRIVATE_CLS(NSURLSession), _NS_PRIVATE_SEL(sessionWithConfiguration_), configuration);
    }

    URLSession* URLSession::session(URLSessionConfiguration* configuration, URLSessionDelegate* delegate, OperationQueue* delegateQueue)
    {
        return Object::sendMessage<URLSession*>(_NS_PRIVATE_CLS(NSURLSession), _NS_PRIVATE_SEL(sessionWithConfiguration_delegate_delegateQueue_), configuration, CreateDelegate(delegate), delegateQueue);
    }

    URLSessionDataTask* URLSession::dataTask(URL* url)
    {
        return Object::sendMessage<URLSessionDataTask*>(this, _NS_PRIVATE_SEL(dataTaskWithURL_), url);
    }
}