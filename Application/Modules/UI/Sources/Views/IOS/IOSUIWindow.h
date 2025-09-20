#pragma once

#include "Templates/IOS/NativeInstance.h"

#include "Views/UIWindow.h"

#include <UIKit/UIKit.h>

@interface IOSWindow : UIWindow

- (instancetype)initWithOwner:(CUIWindow*)Owner;

@end