#include "IOS/IOSUIWindow.h"
#include "UIWindow.h"

#include <CoreGraphics/CGGeometry.h>

#include <UIKit/UIKit.h>

CNativeWindow::CNativeWindow(UIWindow* UIWindow)
:   _uiWindow([UIWindow retain])
{
    
}

CNativeWindow::~CNativeWindow()
{
    [_uiWindow release];
    _uiWindow = nil;
}

CNativeWindow::operator UIWindow*() const
{
    return _uiWindow;
}

CUIWindow::CUIWindow(const SRectF& WindowRect)
{
    CGRect WindowFrame = CGRectMake(WindowRect.X(), WindowRect.Y(), WindowRect.Width(), WindowRect.Height());
    UIWindow* Window = [[UIWindow alloc] initWithFrame:WindowFrame];
    _nativeWindow = MakeShared<CNativeWindow>(Window);
}

CUIWindow::~CUIWindow()
{
   
}

void CUIWindow::Show()
{
    // will be used by the Scene delegate to make it key and visible
    UIApplication.sharedApplication.delegate.window = *_nativeWindow;
}