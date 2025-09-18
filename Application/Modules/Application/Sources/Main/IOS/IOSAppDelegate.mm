#include "IOSApplication.h"

#import <UIKit/UIKit.h>
#include <objc/NSObject.h>

@interface IOSAppDelegate : NSObject <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

- (instancetype)initWithApplication:(CIOSApplication*)application;

@end

@implementation IOSAppDelegate
{
    CIOSApplication* _application;
}

- (instancetype)init
{
    self = [super init];
    if (self) 
    {
        _application = CIOSApplication::Shared();
    }
    return self;
}

- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
    if (_application->Delegate)
    {
        _application->Delegate->DidFinishLaunching();
    }
}

@end