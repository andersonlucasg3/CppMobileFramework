#pragma once

#include "UIViewController.h"

class CNativeNavigationController;

class CUINavigationController : public CUIViewController
{
public:
    UI_API CUINavigationController();
    UI_API ~CUINavigationController();

private:
    TSharedPtr<CNativeNavigationController> _nativeNavigationController;
};