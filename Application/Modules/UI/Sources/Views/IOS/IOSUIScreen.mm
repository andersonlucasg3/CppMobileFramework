#include "Views/UIScreen.h"
#include "IOSUIScreen.h"

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
    UIScreen* Screen = *_nativeScreen;
    CGPoint Origin = Screen.bounds.origin;
    CGSize Size = Screen.bounds.size;

    return SRectF(Origin.x, Origin.y, Size.width, Size.height);
}

CNativeScreen& CUIScreen::NativeScreen()
{
    return *_nativeScreen;
}