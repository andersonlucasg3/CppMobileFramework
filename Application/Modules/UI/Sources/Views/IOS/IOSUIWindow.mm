#include "IOSUIWindow.h"
#include "Views/UIWindow.h"

#include "Controllers/UIViewController.h"
#include "Controllers/IOS/IOSUIViewController.h"

#include <CoreGraphics/CGGeometry.h>
#include <UIKit/UIKit.h>

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

void CUIWindow::SetRootViewController(CUIViewController* InRootViewController)
{
    UIWindow* Window = *_nativeWindow;

    [Window setRootViewController:InRootViewController->NativeViewController()];
}

CNativeWindow& CUIWindow::NativeWindow()
{
    return *_nativeWindow;
}

@implementation IOSWindow

@end