#include "IOSAppDelegate.h"

#include "Application.h"
#include "ApplicationDelegate.h"
#include "IOSApplication.h"
#include "IOSSceneDelegate.h"

#include <UIKit/UIKit.h>

@implementation IOSAppDelegate
{
    CApplicationDelegateWeakObjectPtr _delegate;
}

- (instancetype)init
{
    self = [super init];
    if (self) 
    {
        _delegate = CApplication::SharedApp()->Delegate;
    }
    return self;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
    if (_delegate.IsValid())
    {
        _delegate->DidFinishLaunching(CApplication::SharedApp());
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