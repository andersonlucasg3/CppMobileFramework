#include "IOSUIViewController.h"
#include "Controllers/UIViewController.h"

#include "Templates/IOS/CreateViewController.h"
#include "Views/IOS/IOSUIView.h"
#include "Views/UIView.h"

CUIViewController::CUIViewController()
:   CUIViewController(CreateViewController<IOSViewController>(this))
{
    _view = new CUIView(_nativeViewController.Raw());
}

void CUIViewController::ViewDidLoad()
{
    // to be overriden
}

CUIView* CUIViewController::View() const
{
    return _view;
}

void CUIViewController::SetView(CUIView* View)
{
    _view = View;

    [NativeInstance<IOSViewController>() setView:View->NativeInstance<IOSView>()];
}

@implementation IOSViewController
{
    CUIViewControllerWeakObjectPtr _weakOwner;
}

- (instancetype)initWithOwner:(CUIViewController*)Owner 
{
    if (self = [super init])
    {
        _weakOwner = Owner;
    }
    return self;
}

- (void)viewDidLoad 
{
    [super viewDidLoad];

    if (_weakOwner.IsValid())
    {
        _weakOwner->ViewDidLoad();
    }
}

@end