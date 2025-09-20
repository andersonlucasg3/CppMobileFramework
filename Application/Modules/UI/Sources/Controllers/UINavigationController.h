#pragma once

#include "Object/ClassMacros.h"
#include "UIViewController.h"

FORWARD_DECLARE_OBJECT(UINavigationController);

class CNativeNavigationController;

class CUINavigationController : public CUIViewController
{
    using Super = CUIViewController;

public:
    UI_API CUINavigationController();
    UI_API CUINavigationController(CUIViewController* InRootViewController);
    UI_API ~CUINavigationController() override = default;;

private:
    PROPERTY(CUIViewController, _rootViewController);
};