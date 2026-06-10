#pragma once

#include "Application.h"

class CHelloWorldApp : public CApplication
{
public:
    CUIWindow* CreateMainWindow() override;
};