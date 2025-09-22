#pragma once

#include "Views/UIStackView.h"

#include <UIKit/UIKit.h>

@interface IOSStackView : UIStackView

- (instancetype)initWithOwner:(CUIStackView*)Owner;

@end