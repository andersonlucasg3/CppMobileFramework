#include "IOSSceneDelegate.h"
#include "Application.h"
#include "Views/IOS/IOSUIWindow.h"
#include "Views/UIWindow.h"

#import <UIKit/UIKit.h>

@implementation IOSSceneDelegate
{
    @public CApplicationWeakObjectPtr _application;
}

- (instancetype)init 
{
    if (self = [super init])
    {
        _application = CApplication::SharedApp();
    }
    return self;
}

- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions 
{
    if ([scene isKindOfClass:[UIWindowScene class]])
    {
        CUIWindow* MainWindow = _application->CreateMainWindow();

        if (MainWindow != nullptr && !MainWindow->IsQueuedForDestruction())
        {
            UIWindowScene* WindowScene = (UIWindowScene*)scene;
            IOSWindow* Window = MainWindow->NativeInstance<IOSWindow>();
            [Window setWindowScene:WindowScene];
            [Window makeKeyAndVisible];
        }
    }
}

@end