#pragma once

#include "Application.h"
#include "Templates/IOS/NativeInstance.h"

#include <UIKit/UIKit.h>

@interface IOSApplication : UIApplication

@end

class CNativeApplication : public TNativeInstance<UIApplication> 
{
public:
    static void SetNativeApp(CApplication* App, IOSApplication* IOSApplication);
};