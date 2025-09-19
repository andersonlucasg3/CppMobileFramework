#pragma once

#include "Controllers/UINavigationController.h"
#include "Templates/IOS/NativeInstance.h"

#include <UIKit/UIKit.h>

@interface IOSNavigationController : UINavigationController

@end

class CNativeNavigationController : public TNativeInstance<IOSNavigationController> { };