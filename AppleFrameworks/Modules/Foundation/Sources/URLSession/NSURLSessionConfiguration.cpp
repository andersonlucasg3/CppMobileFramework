#include "NSURLSessionConfiguration.h"

#include "NSPrivate.h"

#if __OBJC__
#include <Foundation/NSURLSession.h>
#endif // __OBJC__

namespace NS
{
    URLSessionConfiguration* URLSessionConfiguration::defaultSessionConfiguration()
    {
#if __OBJC__
        return (URLSessionConfiguration*)[NSURLSessionConfiguration defaultSessionConfiguration];
#else
        return nullptr;
#endif // __OBJC__
    }

    URLSessionConfiguration* URLSessionConfiguration::ephemeralSessionConfiguration()
    {
#if __OBJC__
        return (URLSessionConfiguration*)[NSURLSessionConfiguration ephemeralSessionConfiguration];
#else
        return nullptr;
#endif // __OBJC__
    }
}