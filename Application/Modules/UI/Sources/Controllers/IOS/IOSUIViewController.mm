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

void CUIViewController::ViewWillUnload()
{
    // to be overriden
}

void CUIViewController::ViewDidUnload()
{
    // to be overriden
}

void CUIViewController::ViewWillAppear(bool bAnimated)
{
    // to be overriden
}

void CUIViewController::ViewDidAppear(bool bAnimated)
{
    // to be overriden
}

void CUIViewController::ViewWillDisappear(bool bAnimated)
{
    // to be overriden
}

void CUIViewController::ViewDidDisappear(bool bAnimated)
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

- (void)viewWillUnload 
{
    if (_weakOwner.IsValid())
    {
        _weakOwner->ViewWillUnload();
    }
}

- (void)viewDidUnload 
{
    if (_weakOwner.IsValid())
    {
        _weakOwner->ViewDidUnload();
    }
}

- (void)viewWillAppear:(BOOL)animated 
{
    if (_weakOwner.IsValid())
    {
        _weakOwner->ViewWillAppear(animated);
    }
}

- (void)viewDidAppear:(BOOL)animated
{
    if (_weakOwner.IsValid())
    {
        _weakOwner->ViewDidAppear(animated);
    }
}

- (void)viewWillDisappear:(BOOL)animated 
{
    if (_weakOwner.IsValid())
    {
        _weakOwner->ViewWillDisappear(animated);
    }
}

- (void)viewDidDisappear:(BOOL)animated 
{
    if (_weakOwner.IsValid())
    {
        _weakOwner->ViewDidDisappear(animated);
    }
}

@end