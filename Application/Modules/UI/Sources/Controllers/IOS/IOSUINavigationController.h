#pragma once

#include "Templates/IOS/NativeInstance.h"

#include "Controllers/UINavigationController.h"

#include <UIKit/UIKit.h>

@class IOSViewController;

@interface IOSNavigationController : UINavigationController

- (instancetype)initWithOwner:(CUINavigationController*)Owner;
- (instancetype)initWithOwner:(CUINavigationController*)Owner andRootViewController:(IOSViewController*)RootViewController;

@end