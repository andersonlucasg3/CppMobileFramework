#pragma once

#include "Templates/IOS/NativeInstance.h"

#include "Views/UIView.h"

#include <UIKit/UIKit.h>

@interface IOSView : UIView

- (instancetype)initWithOwner:(CUIView*)Owner;

@end

class CNativeInstance : public TNativeInstance<IOSView> { };