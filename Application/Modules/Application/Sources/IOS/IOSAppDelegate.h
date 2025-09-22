#pragma once

#include "Application.h"

#import <UIKit/UIKit.h>

@interface IOSAppDelegate : NSObject <UIApplicationDelegate>

- (instancetype)initWithApplication:(CApplication*)Application;

@end