#include "Application.h"
#include "IOSApplication.h"
#include "IOSAppDelegate.h"

#include "Object/Collector/CollectorScope.h"

static CApplication* GSharedApp = nullptr;

void CNativeApplication::SetNativeApp(CApplication* App, IOSApplication *IOSApplication)
{
    if (App && IOSApplication)
    {
        App->_nativeApplication = MakeShared<CNativeApplication>(IOSApplication);
    }
}

CApplication* CApplication::SharedApp()
{
    return GSharedApp;
}

int CApplication::Run(int argc, char* argv[])
{
    GSharedApp = this;

    @autoreleasepool
    {
        SCollectorScope Scope;

        NSString* ApplicationClassName = NSStringFromClass([IOSApplication class]);
        NSString* AppDelegateClassName = NSStringFromClass([IOSAppDelegate class]);

        return UIApplicationMain(argc, argv, ApplicationClassName, AppDelegateClassName);
    }
}

@implementation IOSApplication

- (instancetype)init 
{
    self = [super init];
    if (self) 
    {
        CNativeApplication::SetNativeApp(GSharedApp, self);

        self.delegate = [[IOSAppDelegate alloc] initWithApplication:GSharedApp];
    }
    return self;
}

- (void)dealloc 
{
    [self.delegate release];
    self.delegate = nil;

    [super dealloc];
}

@end