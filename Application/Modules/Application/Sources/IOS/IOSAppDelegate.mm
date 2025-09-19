#include "IOSAppDelegate.h"

#include "Application.h"
#include "IOSApplication.h"
#include "IOSSceneDelegate.h"

#include <UIKit/UIKit.h>

@implementation IOSAppDelegate
{
    CApplication* _application;
}

- (instancetype)init
{
    self = [super init];
    if (self) 
    {
        _application = CApplication::SharedApp();
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
    Configuration.delegateClass = [IOSSceneDelegate class];
    Configuration.storyboard = nil;
    return Configuration;
}

@end