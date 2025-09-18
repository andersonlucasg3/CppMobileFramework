#pragma once

#import <UIKit/UIWindow.h>

class CNativeWindow
{
public:
    UI_API CNativeWindow(UIWindow* UIWindow);
    UI_API ~CNativeWindow();

    operator UIWindow*() const;

private:
    UIWindow* _uiWindow;
};