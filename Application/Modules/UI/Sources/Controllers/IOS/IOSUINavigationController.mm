#include "IOSUINavigationController.h"
#include "Controllers/UINavigationController.h"
#include "IOSUIViewController.h"
#include "Templates/IOS/CreateViewController.h"

CUINavigationController::CUINavigationController()
:   Super(CreateNavigationController<IOSNavigationController, CUINavigationController>(this))
{

}


CUINavigationController::CUINavigationController(CUIViewController* InRootViewController)
:   Super(CreateNavigationController<IOSNavigationController>(this, InRootViewController->NativeInstance<IOSViewController>()))
{

}

@implementation IOSNavigationController
{
    @public CUINavigationControllerWeakObjectPtr _weakOwner;
}

- (instancetype)initWithOwner:(CUINavigationController *)Owner 
{
    if (self = [super init])
    {
        _weakOwner = Owner;
    }
    return self;
}

- (instancetype)initWithOwner:(CUINavigationController *)Owner andRootViewController:(IOSViewController *)RootViewController 
{
    if (self = [super initWithRootViewController:RootViewController])
    {
        _weakOwner = Owner;
    }
    return self;
}

@end