#include "IOSUIWindow.h"

#include "Graphics/IOS/IOSGraphics.h"
#include "Templates/IOS/CreateView.h"
#include "Views/IOS/IOSUIView.h"

#include "Controllers/IOS/IOSUIViewController.h"

#include <CoreGraphics/CGGeometry.h>
#include <UIKit/UIKit.h>

CUIWindow::CUIWindow()
:   Super(CreateView<IOSWindow>(this))
{

}

CUIWindow::CUIWindow(const SRectF& WindowRect)
:   Super(CreateView<IOSWindow>(this, WindowRect))
{
    
}

void CUIWindow::SetRootViewController(CUIViewController* InRootViewController)
{
    _rootViewController = InRootViewController;
    
    [NativeInstance<IOSWindow>() setRootViewController:InRootViewController->NativeInstance<IOSViewController>()];
}

@implementation IOSWindow
{
    @public CUIWindowWeakObjectPtr _weakOwner;
}

- (instancetype)initWithOwner:(CUIWindow *)Owner 
{
    if (self = [super init])
    {
        _weakOwner = Owner;
    }
    return self;
}

@end