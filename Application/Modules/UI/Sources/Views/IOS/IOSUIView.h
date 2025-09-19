#pragma once

#include "Templates/IOS/NativeInstance.h"

#include <UIKit/UIKit.h>

@interface IOSView : UIView

@end

class CNativeView : TNativeInstance<IOSView> { };