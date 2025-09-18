#pragma once

#include "Graphics/Rects.h"
#include "UIScreen.h"

#import <UIKit/UIScreen.h>

using namespace UI::Graphics;

class CNativeScreen
{
public:
    UI_API CNativeScreen(UIScreen* UIScreen);
    UI_API ~CNativeScreen();

    operator UIScreen*() const;

private:
    UIScreen* _uiScreen;
};