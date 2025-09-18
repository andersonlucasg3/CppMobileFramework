#include "ObjCIOSSceneDelegate.h"

#import <UIKit/UIKit.h>

@implementation ObjCIOSSceneDelegate

- (void)scene:(UIScene *)scene willConnectToSession:(UISceneSession *)session options:(UISceneConnectionOptions *)connectionOptions 
{
    if ([scene isKindOfClass:[UIWindowScene class]])
    {
        UIWindowScene* WindowScene = (UIWindowScene*)scene;
        UIWindow* Window = UIApplication.sharedApplication.delegate.window;
        [Window setBackgroundColor:[UIColor whiteColor]];
        [Window setWindowScene:WindowScene];
        Window.rootViewController = [[UIViewController alloc] init];
        [Window makeKeyAndVisible];
    }
}

@end