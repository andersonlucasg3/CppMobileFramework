#include "IOSUIScreen.h"

#include "Graphics/IOS/IOSGraphics.h"
#include "Graphics/Rects.h"

#include <CoreFoundation/CFCGTypes.h>
#import <UIKit/UIScreen.h>

CUIScreen* CUIScreen::MainScreen()
{
    CUIScreen* uiScreen = new CUIScreen();
    uiScreen->_nativeScreen = MakeShared<CNativeScreen>([UIScreen mainScreen]);
    return uiScreen;
}

SRectF CUIScreen::Bounds() const
{
    return ToRectF([NativeScreen() bounds]);
}

CNativeScreen& CUIScreen::NativeScreen() const
{
    return *_nativeScreen;
}