#pragma once

#include "Application.h"

#include <UIKit/UIKit.h>

@interface IOSApplication : UIApplication

@end

class CNativeApplication
{
public:
    CNativeApplication(IOSApplication* IOSApplication);
    ~CNativeApplication();

    operator IOSApplication*();

    static void SetNativeApp(CApplication* App, IOSApplication* IOSApplication);

private:
    IOSApplication* _uiApplication;
};