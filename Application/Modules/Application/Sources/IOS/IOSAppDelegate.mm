#include "IOSAppDelegate.h"

#include "IOSApplication.h"
#include "IOSSceneDelegate.h"

#include <UIKit/UIKit.h>

@implementation IOSAppDelegate
{
    CApplicationWeakObjectPtr _application;
}

- (instancetype)initWithApplication:(CApplication *)Application
{
    self = [super init];
    if (self) 
    {
        _application = Application;
    }
    return self;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
    [[NSUserDefaults standardUserDefaults] setBool:YES forKey:@"UIViewDisableAutoLayout"];

    if (_application)
    {
        _application->DidLaunch();
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