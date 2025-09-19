#pragma once

#include "Templates/IOS/NativeInstance.h"
#include <UIKit/UIKit.h>

@interface IOSViewController : UIViewController

@end

class CNativeViewController : public TNativeInstance<IOSViewController> { };