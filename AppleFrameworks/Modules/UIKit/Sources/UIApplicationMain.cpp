#include "UIApplicationMain.h"

#if __OBJC__
@class NSString;

extern "C" int UIApplicationMain(int argc, char * argv[], NSString * principalClassName, NSString * delegateClassName);
#endif

namespace UI 
{
    int ApplicationMain(int argc, char **argv, String *principalClassName, String *delegateClassName)
    {
#if __OBJC__
        return UIApplicationMain(argc, argv, (NSString*)principalClassName, (NSString*)delegateClassName);
#else
        return -1;
#endif
    }
}