#include "NSURLSessionDelegateImpl.h"

#include "URL/NSURLAuthenticationChallenge.h"

#include "URLSession/NSURLSession.h"

using namespace NS;

@implementation NSURLSessionDelegateImpl

- (instancetype)initWithDelegate:(URLSessionDelegate*) InDelegate
{
    if (self = [super init])
    {
        Delegate = InDelegate;
    }

    return self;
}

- (void)dealloc
{
    Delegate = nullptr;
    
    [super dealloc];
}

- (void) URLSession:(NSURLSession *) session didBecomeInvalidWithError:(NSError *) error
{
    if (Delegate)
    {
        Delegate->URLSessionDidBecomeInvalidWithError((URLSession*)session, (Error*)error);
    }
}

- (void) URLSessionDidFinishEventsForBackgroundURLSession:(NSURLSession *) session
{
    if (Delegate)
    {
        Delegate->URLSessionDidFinishEventsForBackgroundURLSession((URLSession*)session);
    }
}

- (void) URLSession:(NSURLSession *) session didReceiveChallenge:(NSURLAuthenticationChallenge *) challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential * credential)) completionHandler
{
    if (Delegate)
    {
        Delegate->URLSessionDidReceiveChallenge((URLSession*)session, (URLAuthenticationChallenge*)challenge, [completionHandler](URLSessionAuthChallengeDisposition disposition, URLCredential* credential)
        {
            completionHandler((NSURLSessionAuthChallengeDisposition)disposition, (NSURLCredential*)credential);
        });
    }
}

@end

namespace NS
{
    void* URLSession::CreateDelegate(URLSessionDelegate* delegate)
    {
        return [[NSURLSessionDelegateImpl alloc] initWithDelegate:delegate];
    }
}