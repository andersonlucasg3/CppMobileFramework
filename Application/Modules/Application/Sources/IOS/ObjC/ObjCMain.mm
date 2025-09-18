#include "ObjCMain.h"

#import <UIKit/UIKit.h>

#import "ObjCIOSApplication.h"
#import "ObjCIOSAppDelegate.h"

int CallMain(int argc, char* argv[])
{
    @autoreleasepool 
    {
        NSString* ApplicationClassName = NSStringFromClass([IOSApplication class]);
        NSString* AppDelegateClassName = NSStringFromClass([IOSAppDelegate class]);
        return UIApplicationMain(argc, argv, ApplicationClassName, AppDelegateClassName);
    }
}