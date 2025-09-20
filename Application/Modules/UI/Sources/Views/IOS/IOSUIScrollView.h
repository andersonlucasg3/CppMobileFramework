#pragma once

#include "Templates/IOS/NativeInstance.h"

#include "Views/UIScrollView.h"

#include <UIKit/UIKit.h>

@interface IOSScrollView : UIScrollView

- (instancetype)initWithOwner:(CUIScrollView*)Owner;

@end