#include "Application.h"
#include "IOSApplication.h"
#include "IOSAppDelegate.h"

#include "Object/Collector/CollectorScope.h"

static CApplication* GSharedApp = nullptr;

CNativeApplication::CNativeApplication(IOSApplication* IOSApplication)
:   _uiApplication([IOSApplication retain])
{

}

CNativeApplication::~CNativeApplication()
{
    [_uiApplication release];
    _uiApplication = nil;
}

CNativeApplication::operator IOSApplication*()
{
    return _uiApplication;
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
        self.delegate = [[IOSAppDelegate alloc] init];

        CNativeApplication::SetNativeApp(GSharedApp, self);
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