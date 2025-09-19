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

    static void SetNativeApp(CApplication* App, IOSApplication* IOSApplication);

    operator IOSApplication*();

private:
    IOSApplication* _uiApplication;
};