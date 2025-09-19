#pragma once

#import <UIKit/UIKit.h>

@interface IOSAppDelegate : NSObject <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

- (instancetype)init;

@end