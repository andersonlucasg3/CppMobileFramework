#pragma once

#if __OBJC__

#import <UIKit/UIKit.h>

@interface IOSAppDelegate : NSObject <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

- (instancetype)init;

@end

#endif