#pragma once

#include "Templates/IOS/NativeInstance.h"

#include <UIKit/UIKit.h>

@interface IOSWindow : UIWindow

@end

class CNativeWindow : public TNativeInstance<IOSWindow> { };