#include "IOSUIScreen.h"

#include "Graphics/Rects.h"
#include "UIScreen.h"

#include <CoreFoundation/CFCGTypes.h>
#import <UIKit/UIScreen.h>

CNativeScreen::CNativeScreen(UIScreen* UIScreen)
:   _uiScreen([UIScreen retain])
{

}

CNativeScreen::~CNativeScreen()
{
    [_uiScreen release];
    _uiScreen = nil;
}

CNativeScreen::operator UIScreen*() const
{
    return _uiScreen;
}

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