#pragma once

#include "Views/UITextField.h"
#include <UIKit/UIKit.h>

@interface IOSTextField : UITextField

- (instancetype)initWithOwner:(CUITextField*)Owner;

@end