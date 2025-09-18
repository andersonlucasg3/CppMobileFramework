#include "ObjcIOSAppDelegate.h"

#if __OBJC__

#include "Application.h"

#import "ObjCIOSSceneDelegate.h"
#import <UIKit/UIKit.h>

@implementation IOSAppDelegate
{
    CApplication* _application;
}

- (instancetype)init
{
    self = [super init];
    if (self) 
    {
        _application = CApplication::Shared();
    }
    return self;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
    if (_application->Delegate)
    {
        _application->Delegate->DidFinishLaunching(_application);
    }
}

- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options
{
    UISceneConfiguration* Configuration = [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
    Configuration.delegateClass = [ObjCIOSSceneDelegate class];
    Configuration.storyboard = nil;
    return Configuration;
}

@end

#endif